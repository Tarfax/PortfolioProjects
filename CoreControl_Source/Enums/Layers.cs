using UnityEngine;

public struct Layers {
    public static LayerMask Floor = 1 << 8;
    public static LayerMask Player = 1 << 9;
    public static LayerMask AI = 1 << 10;
    public static LayerMask Pickup = 1 << 11;
    public static LayerMask ParticleEffect = 1 << 12;
    public static LayerMask Projectile = 1 << 13;
    public static LayerMask Wall = 1 << 15;

    public static LayerMask ProjectileLayerNumber = 13;
}
