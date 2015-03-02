#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#if G_VEC_TYPE_ID == G_TYPE_INT
#define G_VEC_TYPE int
#elif G_VEC_TYPE_ID == G_TYPE_FLOAT
#define G_VEC_TYPE float
#define G_IS_TYPE_FLOATING
#elif G_VEC_TYPE_ID == G_TYPE_LONG
#define G_VEC_TYPE long long
#elif G_VEC_TYPE_ID == G_TYPE_DOUBLE
#define G_VEC_TYPE double
#define G_IS_TYPE_FLOATING
#endif

struct G_VEC_IMP_NAME {
public:
	union {
		struct {
			G_VEC_TYPE data[GVEC_N];
		};
#if GVEC_N == 2
		struct {
			G_VEC_TYPE w, h;
		};
#endif

#ifdef G_VEC_PREV_1_CLASS
		struct {
			G_VEC_PREV_1_CLASS G_VEC_PREV_1_NAME;
		};
#endif
#ifdef G_VEC_PREV_2_CLASS
		struct {
			G_VEC_PREV_2_CLASS G_VEC_PREV_2_NAME;
		};
#endif

		struct {
			G_VEC_TYPE x;
			G_VEC_TYPE y;
#if GVEC_N >= 3
			G_VEC_TYPE z;

#if GVEC_N >= 4
			G_VEC_TYPE o;
#endif
#endif
		};
		struct {
			G_VEC_TYPE r;
			G_VEC_TYPE g;
#if GVEC_N >= 3
			G_VEC_TYPE b;

#if GVEC_N >= 4
			G_VEC_TYPE a;
#endif
#endif
		};
	};

	static G_VEC_IMP_NAME zero() {
		return G_VEC_IMP_NAME(0);
	}

	G_VEC_IMP_NAME() {
		/*for (int i = 0; i < GVEC_N; ++i)
			data[i] = 0;*/
	}

	G_VEC_IMP_NAME(const G_VEC_TYPE& con) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] = con;
	}

	bool isZero() const {
		bool ret = true;
		for (int i = 0; ret && i < GVEC_N; ++i)
			ret &= (data[i] == 0.0f);
		return ret;
	}

	inline bool isNonZero() const {
		return !isZero();
	}

	G_VEC_IMP_NAME operator-() const {
		G_VEC_IMP_NAME cpy = *this;
		for (int i = 0; i < GVEC_N; ++i)
			cpy.data[i] = -cpy.data[i];
		return cpy;
	}

	inline G_VEC_IMP_NAME operator+() const {
		return *this;
	}
	G_VEC_IMP_NAME operator+(const G_VEC_IMP_NAME& other) const {
		G_VEC_IMP_NAME cpy = *this;
		for (int i = 0; i < GVEC_N; ++i)
			cpy.data[i] += other[i];
		return cpy;
	}
	G_VEC_IMP_NAME operator-(const G_VEC_IMP_NAME& other) const {
		G_VEC_IMP_NAME cpy = *this;
		for (int i = 0; i < GVEC_N; ++i)
			cpy.data[i] -= other[i];
		return cpy;
	}
	G_VEC_IMP_NAME operator*(const G_VEC_IMP_NAME& other) const {
		G_VEC_IMP_NAME cpy = *this;
		for (int i = 0; i < GVEC_N; ++i)
			cpy.data[i] *= other[i];
		return cpy;
	}
	G_VEC_IMP_NAME operator/(const G_VEC_IMP_NAME& other) const {
		G_VEC_IMP_NAME cpy = *this;
		for (int i = 0; i < GVEC_N; ++i)
			cpy.data[i] /= other[i];
		return cpy;
	}
	G_VEC_IMP_NAME operator+(const G_VEC_TYPE& con) const {
		G_VEC_IMP_NAME cpy = *this;
		for (int i = 0; i < GVEC_N; ++i)
			cpy.data[i] += con;
		return cpy;
	}
	G_VEC_IMP_NAME operator-(const G_VEC_TYPE& con) const {
		G_VEC_IMP_NAME cpy = *this;
		for (int i = 0; i < GVEC_N; ++i)
			cpy.data[i] -= con;
		return cpy;
	}
	G_VEC_IMP_NAME operator*(const G_VEC_TYPE& con) const {
		G_VEC_IMP_NAME cpy = *this;
		for (int i = 0; i < GVEC_N; ++i)
			cpy.data[i] *= con;
		return cpy;
	}
	G_VEC_IMP_NAME operator/(const G_VEC_TYPE& con) const {
		G_VEC_IMP_NAME cpy = *this;
		for (int i = 0; i < GVEC_N; ++i)
			cpy.data[i] /= con;
		return cpy;
	}
	G_VEC_IMP_NAME& operator=(const G_VEC_IMP_NAME& other) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] = other[i];
		return *this;
	}
	G_VEC_IMP_NAME& operator=(const G_VEC_TYPE& con) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] = con;
		return *this;
	}
	G_VEC_IMP_NAME& operator+=(const G_VEC_IMP_NAME& other) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] += other[i];
		return *this;
	}
	G_VEC_IMP_NAME& operator-=(const G_VEC_IMP_NAME& other) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] -= other[i];
		return *this;
	}
	G_VEC_IMP_NAME& operator/=(const G_VEC_IMP_NAME& other) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] /= other[i];
		return *this;
	}
	G_VEC_IMP_NAME& operator*=(const G_VEC_IMP_NAME& other) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] *= other[i];
		return *this;
	}

	G_VEC_IMP_NAME& operator+=(const G_VEC_TYPE& con) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] += con;
		return *this;
	}
	G_VEC_IMP_NAME& operator-=(const G_VEC_TYPE& con) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] -= con;
		return *this;
	}
	G_VEC_IMP_NAME& operator*=(const G_VEC_TYPE& con) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] *= con;
		return *this;
	}
	G_VEC_IMP_NAME& operator/=(const G_VEC_TYPE& con) {
		for (int i = 0; i < GVEC_N; ++i)
			data[i] /= con;
		return *this;
	}

	bool operator==(const G_VEC_IMP_NAME& other) const {
		bool ret = true;
		for (int i = 0; i < GVEC_N && ret; ++i)
			ret &= (other[i] == data[i]);
		return ret;
	}

	inline G_VEC_TYPE& operator[](int index) {
		return ((G_VEC_TYPE*)this)[index];
	}

	inline G_VEC_TYPE const& operator[](int index) const {
		return ((G_VEC_TYPE*)this)[index];
	}

	G_VEC_TYPE length() const {
		G_VEC_TYPE ret = 0;
		for (int i = 0; i < GVEC_N; ++i)
			ret += data[i] * data[i];
#if G_VEC_TYPE_ID == G_TYPE_INT || G_VEC_TYPE_ID == G_TYPE_LONG
		return (G_VEC_TYPE)sqrt((double)ret);
#else
		return sqrt(ret);
#endif
	}

	G_VEC_TYPE lengthSquared() const {
		G_VEC_TYPE ret = 0;
		for (int i = 0; i < GVEC_N; ++i)
			ret += data[i] * data[i];
		return ret;
	}

	static G_VEC_TYPE length(const G_VEC_IMP_NAME& vec) {
		G_VEC_TYPE ret = 0;
		for (int i = 0; i < GVEC_N; ++i)
			ret += vec.data[i] * vec.data[i];
#if G_VEC_TYPE_ID == G_TYPE_INT || G_VEC_TYPE_ID == G_TYPE_LONG
		return (G_VEC_TYPE)sqrt((double)ret);
#else
		return sqrt(ret);
#endif
	}

	static G_VEC_TYPE lengthSquared(const G_VEC_IMP_NAME& vec) {
		G_VEC_TYPE ret = 0;
		for (int i = 0; i < GVEC_N; ++i)
			ret += vec.data[i] * vec.data[i];
		return ret;
	}

	G_VEC_TYPE distance(const G_VEC_IMP_NAME& other) const {
		G_VEC_TYPE ret = 0;
		for (int i = 0; i < GVEC_N; ++i) {
			G_VEC_TYPE diff = data[i] - other.data[i];
			ret += diff*diff;
		}
#if G_VEC_TYPE_ID == G_TYPE_INT || G_VEC_TYPE_ID == G_TYPE_LONG
		return (G_VEC_TYPE)sqrt((double)ret);
#else
		return sqrt(ret);
#endif
	}

	G_VEC_TYPE distanceSquared(const G_VEC_IMP_NAME& other) const {
		G_VEC_TYPE ret = 0;
		for (int i = 0; i < GVEC_N; ++i) {
			G_VEC_TYPE diff = data[i] - other.data[i];
			ret += diff*diff;
		}
		return ret;
	}

	static G_VEC_TYPE distance(const G_VEC_IMP_NAME& vec1, const G_VEC_IMP_NAME& vec2) {
		G_VEC_TYPE ret = 0;
		for (int i = 0; i < GVEC_N; ++i) {
			G_VEC_TYPE diff = vec1.data[i] - vec2.data[i];
			ret += diff*diff;
		}
#if G_VEC_TYPE_ID == G_TYPE_INT || G_VEC_TYPE_ID == G_TYPE_LONG
		return (G_VEC_TYPE)sqrt((double)ret);
#else
		return sqrt(ret);
#endif
	}

	static G_VEC_TYPE distanceSquared(const G_VEC_IMP_NAME& vec1, const G_VEC_IMP_NAME& vec2) {
		G_VEC_TYPE ret = 0;
		for (int i = 0; i < GVEC_N; ++i) {
			G_VEC_TYPE diff = vec1.data[i] - vec2.data[i];
			ret += diff*diff;
		}
		return ret;
	}

	//

	G_VEC_TYPE dot(const G_VEC_IMP_NAME& other) const {
		G_VEC_TYPE ret = 0;
		for (int i = 0; i < GVEC_N; ++i)
			ret += data[i] * other.data[i];
		return ret;
	}

	static G_VEC_TYPE dot(const G_VEC_IMP_NAME& vec1, const G_VEC_IMP_NAME& vec2) {
		G_VEC_TYPE ret = 0;
		for (int i = 0; i < GVEC_N; ++i)
			ret += vec1.data[i] * vec2.data[i];
		return ret;
	}


#ifdef G_IS_TYPE_FLOATING
	static G_VEC_IMP_NAME normalize(const G_VEC_IMP_NAME& vec) {
		G_VEC_TYPE d = vec.length();
		G_VEC_IMP_NAME ret;
		for (int i = 0; i < GVEC_N; ++i)
			ret.data[i] = vec.data[i] / d;
		return ret;
	}
	G_VEC_TYPE normalize() {
		G_VEC_TYPE d = length();
		for (int i = 0; i < GVEC_N; ++i)
			data[i] /= d;
		return d;
	}
	G_VEC_IMP_NAME normalized() {
		return G_VEC_IMP_NAME::normalize(*this);
	}
#endif

#if GVEC_N == 2
	G_VEC_IMP_NAME(G_VEC_TYPE x, G_VEC_TYPE y) {
		data[0] = x;
		data[1] = y;
	}
#endif

#if GVEC_N == 3

	static G_VEC_IMP_NAME fromColor(unsigned int color) {
		G_VEC_IMP_NAME ret;

#ifdef G_IS_TYPE_FLOATING
		G_VEC_TYPE d = (G_VEC_TYPE)255;
		ret.r = ((color >> 16) & 255) / d;
		ret.g = ((color >> 8) & 255) / d;
		ret.b = ((color >> 0) & 255) / d;
#else
		ret.r = ((color >> 16) & 255);
		ret.g = ((color >> 8) & 255);
		ret.b = ((color >> 0) & 255);
#endif
		return ret;
	}

	G_VEC_IMP_NAME(G_VEC_TYPE x, G_VEC_TYPE y, G_VEC_TYPE z) {
		data[0] = x;
		data[1] = y;
		data[2] = z;
	}

	G_VEC_IMP_NAME cross(const G_VEC_IMP_NAME& other) const {
		return G_VEC_IMP_NAME(
			y * other.z - other.y * z,
			z * other.x - other.z * x,
			x * other.y - other.x * y);
	}


	static G_VEC_IMP_NAME cross(const G_VEC_IMP_NAME& vec1, const G_VEC_IMP_NAME& vec2) {
		return G_VEC_IMP_NAME(
			vec1.y * vec2.z - vec2.y * vec1.z,
			vec1.z * vec2.x - vec2.z * vec1.x,
			vec1.x * vec2.y - vec2.x * vec1.y);
	}
#endif

#if GVEC_N == 4
	static G_VEC_IMP_NAME fromColor(unsigned int color) {
		G_VEC_IMP_NAME ret;

#ifdef G_IS_TYPE_FLOATING
		G_VEC_TYPE d = (G_VEC_TYPE)255;
		ret.r = ((color >> 16) & 255) / d;
		ret.g = ((color >> 8) & 255) / d;
		ret.b = ((color >> 0) & 255) / d;
		ret.a = ((color >> 24) & 255) / d;
#else
		ret.r = ((color >> 16) & 255);
		ret.g = ((color >> 8) & 255);
		ret.b = ((color >> 0) & 255);
		ret.a = ((color >> 24) & 255);
#endif
		return ret;
	}
	G_VEC_IMP_NAME(G_VEC_PREV_1_CLASS vec, G_VEC_TYPE con) {
		data[0] = vec.x;
		data[1] = vec.y;
		data[2] = vec.z;
		data[3] = con;
	}
	G_VEC_IMP_NAME(G_VEC_TYPE x, G_VEC_TYPE y, G_VEC_TYPE z) {
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = (G_VEC_TYPE)1;
	}
	G_VEC_IMP_NAME(G_VEC_TYPE x, G_VEC_TYPE y, G_VEC_TYPE z, G_VEC_TYPE o) {
		data[0] = x;
		data[1] = y;
		data[2] = z;
		data[3] = o;
	}
#endif
};

#undef G_VEC_TYPE
#ifdef G_IS_TYPE_FLOATING
#undef G_IS_TYPE_FLOATING
#endif