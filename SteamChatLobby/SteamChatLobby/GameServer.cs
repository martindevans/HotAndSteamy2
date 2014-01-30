using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using HotAndSteamy.Extensions;
using Microsoft.Xna.Framework;
using NativeAndSteamy;
using SteamChatLobby.Packets;

namespace SteamChatLobby
{
    public class GameServer
    {
        private readonly IEnumerable<SteamId> _ids;
        private readonly SteamId _localId;
        private readonly HashSet<SteamId> _connectedTo = new HashSet<SteamId>();

        public bool IsFullyConnected
        {
            get { return _connectedTo.SetEquals(_ids); }
        }

        public IEnumerable<SteamId> Players
        {
            get { return _connectedTo; }
        }

        private bool _started = false;
        public float Time { get; private set; }

        private readonly ConcurrentDictionary<SteamId, float> _turningInputs = new ConcurrentDictionary<SteamId, float>();

        private readonly Dictionary<SteamId, ShipState> _ships = new Dictionary<SteamId, ShipState>();
        public IEnumerable<ShipState> Ships
        {
            get { return _ships.Values; }
        }

        private float _lastLatencyUpdate = float.MinValue;
        private readonly Dictionary<SteamId, float> _latency = new Dictionary<SteamId, float>();

        private readonly ConcurrentDictionary<SteamId, uint> _scores = new ConcurrentDictionary<SteamId, uint>();

        private const float PING_PERIOD = 0.25f;
        private readonly ConcurrentDictionary<SteamId, float> _timeouts = new ConcurrentDictionary<SteamId, float>();

        public uint Score(SteamId id)
        {
            uint s;
            if (!_scores.TryGetValue(id, out s))
                return 0;
            return s;
        }

        private float _lastShipUpdate = float.MinValue;

        public const float BULLET_LIFE = 2.5f;
        private const float SPACE_FRICTION = 0.25f;

        public float Latency(SteamId id)
        {
            float l;
            if (!_latency.TryGetValue(id, out l))
                return 0;
            return l;
        }

        public GameServer(IEnumerable<SteamId> ids, SteamId localId)
        {
            _ids = ids;
            _localId = localId;
        }

        public void Update(float dt)
        {
            Time += dt;

            if (!_started)
                Start();

            if (_lastLatencyUpdate + PING_PERIOD <= Time)
                SendPings();

            foreach (var shipState in Ships)
            {
                shipState.Integrate(dt);

                shipState.VelocityX = MathHelper.Lerp(shipState.VelocityX, 0, SPACE_FRICTION * dt);
                shipState.VelocityY = MathHelper.Lerp(shipState.VelocityY, 0, SPACE_FRICTION * dt);

                float turn;
                if (_turningInputs.TryGetValue(new SteamId(shipState.SteamId), out turn))
                    shipState.Angle += turn;
            }

            foreach (var bullet in Bullets)
                bullet.Integrate(dt);
            foreach (var bulletSet in _bullets)
                bulletSet.Value.RemoveWhere(a => a.TimeCreated + BULLET_LIFE <= Time);

            if (_lastShipUpdate + 0.1f <= Time)
            {
                _lastShipUpdate = Time;
                foreach (var shipState in Ships.Where(a => a.IsLocal))
                    foreach (var steamId in _connectedTo)
                        steamId.SendP2PPacket(shipState.ToBytes(), P2PSendTypes.UnreliableNoDelay, (int) ChannelAssignments.ShipState);
            }

            foreach (var id in _timeouts.Keys.ToArray())
            {
                var value = _timeouts.AddOrUpdate(id, dt, (i, v) => v + dt);
                if (value > Math.Max(1, PING_PERIOD * 4))
                    DisconnectedFrom(id);
            }

            foreach (var steamId in _connectedTo)
            {
                var state = steamId.GetP2PSessionState();
                Console.WriteLine(state.UsingRelay);
            }

            ReadMessages();
        }

        private void Start()
        {
            foreach (var steamId in _ids)
            {
                if (!SteamAPI.Instance.SteamNetworking.AcceptP2PSessionWithUser(steamId))
                    throw new InvalidOperationException("Failed to accept p2p session");

                steamId.SendP2PPacket(new IntroMessage().ToBytes(), P2PSendTypes.Reliable, (int)ChannelAssignments.Introductions);
            }

            _started = true;
        }

        private void SendPings()
        {
            _lastLatencyUpdate = Time;
            foreach (var steamId in _ids)
            {
                steamId.SendP2PPacket(new Ping {SendTime = DateTime.Now.Ticks}.ToBytes(), P2PSendTypes.UnreliableNoDelay, (int) ChannelAssignments.Ping);
            }
        }

        //Channel assignments:
        private enum ChannelAssignments
        {
            Introductions,
            Ping,
            ShipState,
            BulletCreation,
            BulletDestruction,
            Pong,
            Turn,
            Disconnect
        }

        private void ReadMessages()
        {
            ReadIntroductions();
            ReadPings();
            ReadStates();
            ReadBulletCreations();
            ReadPongs();
            ReadTurns();
            ReadBulletDestructions();
            ReadLostConnections();
        }

        private static void ReadAllChannelMessages<T>(ChannelAssignments channel, Action<T, SteamId> handler) where T : BasePacket
        {
            uint length;
            do
            {
                length = SteamAPI.Instance.SteamNetworking.IsP2PPacketAvailable((int)channel);
                if (length > 0)
                {
                    ArraySegment<byte> data = new ArraySegment<byte>(new byte[length]);
                    uint length2;
                    SteamId remote;
                    if (SteamAPI.Instance.SteamNetworking.ReadP2PPacket(data, out length2, out remote, (int) channel))
                    {
                        T decoded = (T)BasePacket.FromBytes(new ArraySegment<byte>(data.Array, data.Offset, (int) length2));
                        handler(decoded, remote);
                    }
                    else
                        Console.WriteLine("Failed to read P2P message");
                }
            } while (length != 0);
        }

        private void ReadIntroductions()
        {
            ReadAllChannelMessages<IntroMessage>(ChannelAssignments.Introductions, (a, b) => _connectedTo.Add(b));
        }

        private void ReadPings()
        {
            ReadAllChannelMessages<Ping>(ChannelAssignments.Ping, (a, b) => b.SendP2PPacket(new Pong {SendTime = a.SendTime}.ToBytes(), P2PSendTypes.UnreliableNoDelay, (int)ChannelAssignments.Pong));
        }

        private void ReadPongs()
        {
            ReadAllChannelMessages<Pong>(ChannelAssignments.Pong, (a, b) =>
            {
                _latency[b] = (float) ((TimeSpan.FromTicks(DateTime.Now.Ticks - a.SendTime)).TotalSeconds / 2.0);
                _timeouts.AddOrUpdate(b, 0, (i, v) => 0); //Reset timeout for this player
            });
        }

        private void ReadStates()
        {
            ReadAllChannelMessages<ShipState>(ChannelAssignments.ShipState, (a, b) =>
            {
                a.Integrate(Latency(b));
                _ships[b] = a;
            });
        }

        private void ReadBulletCreations()
        {
            ReadAllChannelMessages<BulletCreation>(ChannelAssignments.BulletCreation, (a, b) =>
            {
                a.TimeCreated = Time - Latency(b);
                a.OwnerId = b;
                GetBulletsForId(b).Add(a);
            });
        }

        private void ReadBulletDestructions()
        {
            ReadAllChannelMessages<BulletDestruction>(ChannelAssignments.BulletDestruction, (a, b) =>
            {
                _scores.AddOrUpdate(new SteamId(a.OwnerId), 1, (x, y) => y + 1);

                HashSet<BulletCreation> bullets;
                if (_bullets.TryGetValue(new SteamId(a.OwnerId), out bullets))
                    bullets.RemoveWhere(x => x.Id == a.Id);
            });
        }

        private void ReadTurns()
        {
            ReadAllChannelMessages<BeginTurn>(ChannelAssignments.Turn, (a, b) =>
            {
                _turningInputs[b] = a.DeltaAngle;
            });
        }

        private void ReadLostConnections()
        {
            ReadAllChannelMessages<LostConnection>(ChannelAssignments.Disconnect, (a, b) =>
            {
                _connectedTo.Remove(new SteamId(a.SteamId));
                SteamAPI.Instance.SteamNetworking.CloseP2PSessionWithUser(new SteamId(a.SteamId));
            });
        }

        public ShipState CreateNewLocalShip(SteamId id)
        {
            var state = new ShipState
            {
                Angle = 0,
                PositionX = 0,
                PositionY = 0,
                SteamId = id.ToUInt64(),
                VelocityX = 0,
                VelocityY = 0,
                IsLocal = true,
            };
            _ships.Add(id, state);

            return state;
        }

        private ushort _localBulletId = 0;
        public void CreateNewLocalBullet(ShipState myShip)
        {
            var v = new Vector2((float) Math.Sin(myShip.Angle) * 100, (float) Math.Cos(myShip.Angle) * 100);
            BulletCreation c = new BulletCreation
            {
                Id = unchecked(_localBulletId++),
                IsLocal = true,
                PositionX = myShip.PositionX,
                PositionY = myShip.PositionY,
                VelocityX = myShip.VelocityX + v.X,
                VelocityY = myShip.VelocityY - v.Y,
                TimeCreated = Time,
            };

            foreach (var steamId in _connectedTo)
                SteamAPI.Instance.SteamNetworking.SendP2PPacket(steamId, c.ToBytes(), P2PSendTypes.UnreliableNoDelay, (int)ChannelAssignments.BulletCreation);

            GetBulletsForId(_localId).Add(c);
        }

        private readonly Dictionary<SteamId, HashSet<BulletCreation>> _bullets = new Dictionary<SteamId, HashSet<BulletCreation>>();

        private HashSet<BulletCreation> GetBulletsForId(SteamId id)
        {
            HashSet<BulletCreation> result;
            if (!_bullets.TryGetValue(id, out result))
            {
                result = new HashSet<BulletCreation>();
                _bullets[id] = result;
            }
            return result;
        }

        public IEnumerable<BulletCreation> Bullets
        {
            get { return _bullets.SelectMany(a => a.Value); }
        }

        public void TurnLocalShip(float f)
        {
            float old = float.NaN;
            _turningInputs.AddOrUpdate(_localId, f, (k, v) => { old = v; return f; });

// ReSharper disable CompareOfFloatsByEqualityOperator
            if (f != old)
// ReSharper restore CompareOfFloatsByEqualityOperator
            {
                foreach (var steamId in _connectedTo)
                    SteamAPI.Instance.SteamNetworking.SendP2PPacket(steamId, new BeginTurn {DeltaAngle = f}.ToBytes(), P2PSendTypes.Reliable, (int) ChannelAssignments.Turn);
            }
        }

        public void BulletHit(BulletCreation b)
        {
            _bullets[b.OwnerId].Remove(b);

            BulletDestruction d = new BulletDestruction
            {
                Id = b.Id,
                OwnerId = b.OwnerId.ToUInt64(),
            };

            _scores.AddOrUpdate(b.OwnerId, 1, (x, y) => y + 1);

            foreach (var steamId in _connectedTo)
                SteamAPI.Instance.SteamNetworking.SendP2PPacket(steamId, d.ToBytes(), P2PSendTypes.ReliableWithBuffer, (int)ChannelAssignments.BulletDestruction);
        }

        private void DisconnectedFrom(SteamId accountId)
        {
            _connectedTo.Remove(accountId);
            SteamAPI.Instance.SteamNetworking.CloseP2PSessionWithUser(accountId);

            foreach (var steamId in _connectedTo)
                SteamAPI.Instance.SteamNetworking.SendP2PPacket(steamId, new LostConnection { SteamId = accountId.ToUInt64() }.ToBytes(), P2PSendTypes.Reliable, (int)ChannelAssignments.Disconnect);
        }
    }
}
