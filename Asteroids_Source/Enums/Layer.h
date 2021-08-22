enum class Layer {
	Nothing = 1 << 0,
	Asteroid = 1 << 1,
	Player = 1 << 2,
	Projectile = 1 << 3,
	UFO = 1 << 4,
	UFOProjectile = 1 << 5,
};

inline Layer operator|(Layer a, Layer b) {
	return static_cast<Layer>(static_cast<int>(a) | static_cast<int>(b));
}