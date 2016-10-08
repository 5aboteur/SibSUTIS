typedef struct Material {
	float4 color;

	float reflectivity;
	float refractivity;
	float4 emissivity;
	float ior;
}Material;

typedef struct Hit_record {
	float4 point;
	float4 normal;

	float depth;
	Material material;
}Hit_record;

typedef struct Ray {
	float4 origin;
	float4 direction;
}Ray;

typedef struct Sphere {
	float4 center;
	float radius;
	Material material;
}Sphere;

typedef struct Camera {
	float4 lowleftcorner;
	float4 horizontal;
	float4 vertical;
	float4 origin;
}Camera;

int random_number_kernel(int *seed) {
    int const a = 16807; //ie 7**5
    int const m = 2147483646; //ie 2**31-1

    *seed = ( (long)(*seed * a)) % m;
    return (*seed);
}

int rand(__global int* seed_memory) {
    int global_id = get_global_id(1) * get_global_size(0) + get_global_id(0);
	int seed = seed_memory[global_id];
	int result = seed;
	result = result & 0x7FFFFFFF;
	int random_number = random_number_kernel(&seed);
	seed_memory[global_id] = seed;
	return result;
}

float drand(__global int* seed_memory) {
	return rand(seed_memory) / (float)2147483646;
}

/********************* KERNEL *********************/

bool refraction(float4 v1, float4 v2, float nu, float4 *refr) {
	float4 uv = normalize(v1);
	float t = dot(uv, v2);
	float D = 1.0f - nu * nu * (1.0f - t * t);

	if (D > 0.0f) {
		*refr = nu * (v1 - v2 * t) - v2 * sqrt(D);
		return true;
	}
	else 
		return false;
}

float4 reflection(float4 v1, float4 v2) {
	return v1 - 2.0f * dot(v1, v2) * v2;
}

float shlick_approximation(float cosine, float refract) {
	float p = (1.0f - refract) / (1.0f + refract);
	p *= p;
	return p + (1.0f - p) * pow((1.0f - cosine), 5);
}

float4 get_ray(Ray r, float k) {
	return r.origin + k * r.direction;
}

Ray shoot_ray(Camera cam, float w, float h) {
	Ray r = { cam.origin, cam.lowleftcorner + w*cam.horizontal + h*cam.vertical - cam.origin };
	return r;
}

bool diffusion(Ray r, Hit_record rec, float4 *fade, Ray *scat, __global int* seed_memory) {
	if (length(rec.material.emissivity) > 0.0f) return false;

	if (rec.material.reflectivity == 0.0f && rec.material.refractivity == 0.0f) {
		// matte

		// float4 t;

		// do {
		// 	t = 2.0f * (float4) {drand(seed_memory), drand(seed_memory), drand(seed_memory), 0} - (float4) {1.0f, 1.0f, 1.0f, 0.0f};
		// } while (t.x*t.x + t.y*t.y + t.z*t.z >= 1.0f);

		// float4 target = rec.point + rec.normal + t;
		// Ray scat_ray = { rec.point, target - rec.point};
		float4 v1 = {drand(seed_memory) * 2.0 - 1.0, drand(seed_memory) * 2.0 - 1.0, drand(seed_memory) * 2.0 - 1.0, 0};
		v1 = normalize(v1);
		if (dot(v1, rec.normal) < 0.0f) v1 = -v1;
		Ray scat_ray = { rec.point, v1};
		*scat = scat_ray;
		*fade = rec.material.color;

		return true;
	} else if (rec.material.reflectivity > 0.0f && rec.material.refractivity == 0.0f) {
	// 	// metal

		float4 refl = reflection(normalize(r.direction), rec.normal);
		Ray scat_ray = { rec.point, refl };
		*scat = scat_ray;
		*fade = rec.material.color;
		return (dot(scat->direction, rec.normal) > 0);
	} 
	else {
		// glass

		float4 out_normal;
		float4 refr;
		float4 refl = reflection(normalize(r.direction), rec.normal);

		float nu, prob, cosi;

		*fade = (float4){1.0f, 1.0f, 1.0f, 0.0f};
		
		if (dot(r.direction, rec.normal) > 0) {
			out_normal = -rec.normal;
			nu = rec.material.ior;
			cosi = dot(r.direction, rec.normal) / length(r.direction);
			cosi = sqrt(1.0f - rec.material.ior*rec.material.ior * (1.0f - cosi*cosi));
		} else {
			out_normal = rec.normal;
			nu = 1.0f / rec.material.ior;
			cosi = -dot(r.direction, rec.normal) / length(r.direction);
		}

		float4 v1 = r.direction;
		float4 v2 = out_normal;
		float4 uv = normalize(v1);
		float t = dot(uv, v2);
		float D = 1.0f - nu * nu * (1.0f - t * t);

		if (D > 0.0f) {
			refr = nu * (v1 - v2 * t) - v2 * sqrt(D);
			prob = shlick_approximation(cosi, rec.material.ior);
		} else prob = 1.0f;

		if (drand(seed_memory) < prob) {
			Ray r = { rec.point, refl };
			*scat = r;
		} else {
			Ray r = { rec.point, refr };
			*scat = r;
		}
		
		return true;
	}

	// return true;
}

bool get_intersection(Ray r, __global Sphere *s, float min_depth, float max_depth, Hit_record *rec) {
	float4 oc;
	float4 ro = r.origin;
	float4 ce = s->center;
	oc = ro - ce;
	
	float a = dot(r.direction, r.direction);
	float b = 2.0f * dot(oc, r.direction);
	float c = dot(oc, oc) - s->radius * s->radius;
	float D = b*b - 4.0f*a*c;

	if (D > 0) {
		float d1 = (-b - sqrt(D)) / (2.0f * a);
		float d2 = (-b + sqrt(D)) / (2.0f * a);
		float root = (d1 < d2) ? d1 : d2;

		if (root < max_depth && root > min_depth) {	
			rec->point = get_ray(r, root);
			rec->normal = (get_ray(r, root) - ce) / s->radius;
			rec->depth = root;
			rec->material = s->material;
			
			return true;
		}
	}
	
	return false;
}

bool nearest_intersection(Ray r, __global Sphere *scene, float min_depth, float max_depth, Hit_record *rec, int n) {
	Hit_record tmp;

	bool hit = false;
	float dist = max_depth;

	for (int i = 0; i < n; ++i) {
		if (get_intersection(r, &scene[i], min_depth, dist, &tmp)) {
			hit = true;
			dist = tmp.depth;
			*rec = tmp;
		}
	}

	return hit;
}


float4 raytrace(Ray r, __global Sphere *scene, __global int *seed_memory, int nobj) {
	Ray primary = r;
	Ray scattered;
	float4 attenuated;
	float4 emitted;
	float4 composite = {0.0f, 0.0f, 0.0f, 0.0f};
	float4 counted = {1.0f, 1.0f, 1.0f, 0.0f};

	Hit_record rec;
	for (int i = 0; i < 128; ++i) {	
		if (nearest_intersection(primary, scene, 0.001f, 1E+37f, &rec, nobj)) {
			emitted = rec.material.emissivity;
			if (diffusion(primary, rec, &attenuated, &scattered, seed_memory)) {
				primary = scattered;
				composite += emitted * counted;
				counted *= attenuated;
			} else 
				return composite + emitted * counted;
		} else 
			return (float4){0.0f, 0.0f, 0.0f, 0.0f};
	}
	
	return (float4){0.0f, 0.0f, 0.0f, 0.0f};
}

void create_cam(Camera *cam, float4 pov, float4 target, float fov, float aspectratio) {
	cam->lowleftcorner = 0.0f;
	cam->horizontal = 0.0f;
	cam->vertical = 0.0f;
	cam->origin = pov;

	float4 upward = { 0.0f, 1.0f, 0.0f, 0.0f }; 

	float alpha = fov * 3.14159265359f / 180.0f; 
	float vfov = tan(alpha / 2.0f); 
	float hfov = aspectratio * vfov; 

	float4 dz = normalize(pov - target); 
	float4 crossed = cross(upward, dz); 
	float4 dx = normalize(crossed); 
	float4 dy = cross(dz, dx); 

	cam->lowleftcorner.x = cam->origin.x - hfov*dx.x - vfov*dy.x - dz.x; 
	cam->lowleftcorner.y = cam->origin.y - hfov*dx.y - vfov*dy.y - dz.y; 
	cam->lowleftcorner.z = cam->origin.z - hfov*dx.z - vfov*dy.z - dz.z; 

	cam->horizontal.x = 2.0f * hfov * dx.x; 
	cam->horizontal.y = 2.0f * hfov * dx.y; 
	cam->horizontal.z = 2.0f * hfov * dx.z; 

	cam->vertical.x = 2.0f * vfov * dy.x; 
	cam->vertical.y = 2.0f * vfov * dy.y; 
	cam->vertical.z = 2.0f * vfov * dy.z;
}


__kernel void test(__global float4 *result, __global int *seed_memory, __global Sphere *scene) {
	
	int SAMPLES = 100;
	int WIDTH = 1024;
	int gid = get_global_id(0);
	int index = gid;

	float4 zero = 0.0f;
	
	result[gid] = zero;
	float scale = 1.0f / (float)SAMPLES;

	Ray primary;
	float4 gidded, scaled, new_val;

	Camera cam;
	create_cam(&cam, (float4){0.0f, 128.0f, -800.0f, 0.0f}, (float4) {0.0f, 0.0f, 0.0f, 0.0f}, 40.0f, 1.0f);
	primary.origin = cam.origin;
	float4 part1 = cam.lowleftcorner;
	float4 part2 = cam.horizontal;
	float4 part3 = cam.vertical;

	__local float w_off, h_off;

	float4 w, h;

	for (int spp = 0; spp < SAMPLES; ++spp) {
		w_off = (float)(index % WIDTH + drand(seed_memory)) / (float)(WIDTH);
		h_off = (float)(index / WIDTH + drand(seed_memory)) / (float)(WIDTH);

		w = w_off * part2;
		h = h_off * part3;
		primary.direction = part1 + w + h - primary.origin;

		new_val = raytrace(primary, scene, seed_memory, 9);
		scaled = scale * new_val;
		gidded = result[gid];
		result[gid] = scaled + gidded;
	}
}
