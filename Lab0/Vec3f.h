
template<typename T>
class Vec3 {
};


class Vec3f {

friend Vec3f operator*(float i, Vec3f v);

public:
	Vec3f();
	Vec3f(float x, float y, float z);

	Vec3f& operator= (const Vec3f &other);
	Vec3f& operator+ (const Vec3f &other);
	Vec3f& operator- (const Vec3f &other);
	float operator* (const Vec3f &other);
	Vec3f& operator% (const Vec3f &other);

protected:
	float value[3];
};

Vec3f operator*(float i, Vec3f v);
