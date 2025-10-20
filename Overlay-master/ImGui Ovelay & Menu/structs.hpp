
struct Vector3 {
    float x, y, z;

    // Addition
    Vector3 operator+(const Vector3& other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vector3& operator+=(const Vector3& other) {
        x += other.x;
        y += other.y;
        z += other.z;
        return *this;
    }

    // Subtraction
    Vector3 operator-(const Vector3& other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    Vector3& operator-=(const Vector3& other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
        return *this;
    }

    // Scalar multiplication
    Vector3 operator*(float scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }

    Vector3& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }

    // Scalar division
    Vector3 operator/(float scalar) const {
        return { x / scalar, y / scalar, z / scalar };
    }

    Vector3& operator/=(float scalar) {
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }

    // Optional: negate
    Vector3 operator-() const {
        return { -x, -y, -z };
    }
};

struct view_matrix_t {
	float* operator[ ](int index) {
		return matrix[index];
	}

	float matrix[4][4];
};