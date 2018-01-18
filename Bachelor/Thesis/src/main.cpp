#include <fstream>
#include <iostream>
#include <string>

#include <cfloat>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#define MAX_SOURCE_SIZE (0x100000)
#include <CL/cl.h>

#define WIDTH		256
#define HEIGHT		256
#define ASPECT		WIDTH / HEIGHT
#define NPIX		WIDTH * HEIGHT

#define SAMPLES		1024
#define SCALE		1.0F / SAMPLES
#define MAX_DEPTH	1
#define NOBJ		9

typedef struct Material {
	cl_float4 color;

 	float reflectivity;
 	float refractivity;
 	cl_float4 emissivity;
 	float ior;
} Material;

 typedef struct Sphere {
 	cl_float4 center;
 	cl_float radius;
 	Material material;
} Sphere;

cl_int init_gpu(cl_device_id &, cl_context &, cl_command_queue &);
cl_int init_kernel(cl_device_id &, cl_context &, cl_kernel &, cl_program &);
cl_int run_kernel(cl_context &, cl_command_queue &, cl_kernel &);
cl_int free_kernel(cl_context &, cl_command_queue &, cl_kernel &, cl_program &);

int gamma_correction(float val);
Sphere *Cornell_Box();

int main(void)
{
	srand((unsigned)time(NULL));

	/* Initialize all the GPU-side stuff */

	cl_device_id		device_id;
	cl_context			context;
	cl_command_queue	command_queue;
	cl_kernel			kernel;
	cl_program			program;
	cl_int				ret;

	std::cout << "Initialize device ... ";
	if (!(ret = init_gpu(device_id, context, command_queue))) std::cout << "OK!\n";
	else {
		std::cout << "Something is going wrong. Error code: " << ret << "\n"; return -1;
	}

	std::cout << "Initialize kernel ... \n";
	if (!(ret = init_kernel(device_id, context, kernel, program))) std::cout << "OK!\n";
	else {
		std::cout << "Something is going wrong. Error code: " << ret << "\n"; return -1;
	}

	std::cout << "Run kernel ... ";
	if (!(ret = run_kernel(context, command_queue, kernel))) std::cout << "OK!\n";
	else {
		std::cout << "Something is going wrong. Error code: " << ret << "\n"; return -1;
	}

	std::cout << "Free kernel`s memory ... \n";
	if (!(ret = free_kernel(context, command_queue, kernel, program))) std::cout << "OK!\n";
	else {
		std::cout << "Something is going wrong. Error code: " << ret << "\n"; return -1;
	}

	return 0;
}

cl_int init_gpu(cl_device_id &device_id, cl_context &context, cl_command_queue &command_queue)
{
	cl_platform_id platform_id;
	cl_uint ret_num_platforms, ret_num_devices;
	cl_int ret;

	ret = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
	ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, &ret_num_devices);

	/* GET INFO */

	size_t retSize;
	cl_ulong size;
	cl_uint mcu;

	bool *isAvail = NULL;
	clGetDeviceInfo(device_id, CL_DEVICE_AVAILABLE, sizeof(cl_bool), &isAvail, 0);
	if (isAvail) std::cout << "DEVICE IS AVAILABLE.\n";


	std::cout << "********************************************\n";

	ret = clGetDeviceInfo(device_id, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &mcu, 0);
	std::cout << " CL_DEVICE_MAX_COMPUTE_UNITS:\t" << mcu << "\n";

	ret = clGetDeviceInfo(device_id, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &size, 0);
	std::cout << " CL_DEVICE_MAX_MEM_ALLOC_SIZE:\t" << size << "\n";

	ret = clGetDeviceInfo(device_id, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &size, 0);
	std::cout << " CL_DEVICE_GLOBAL_MEMORY_SIZE:\t" << size << "\n";

	ret = clGetDeviceInfo(device_id, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &size, 0);
	std::cout << " CL_DEVICE_LOCAL_MEMORY_SIZE:\t" << size << "\n";

	ret = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &retSize, 0);
	std::cout << " CL_DEVICE_MAX_WORK_GROUP_SIZE:\t" << retSize << "\n";

	ret = clGetDeviceInfo(device_id, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &mcu, 0);
	std::cout << " CL_DEVICE_MAX_WORK_ITEM_DIMEN:\t" << mcu << "\n";

	ret = clGetDeviceInfo(device_id, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(cl_ulong), &size, 0);
	std::cout << " CL_DEVICE_MAX_CONST_BUFF_SIZE:\t" << size << "\n";

	std::cout << "********************************************\n";

	/* CREATE CONTEXT & COMMAND QUEUE */

	context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &ret);
	command_queue = clCreateCommandQueue(context, device_id, 0, &ret);

	return ret;
}

cl_int init_kernel(cl_device_id &device_id, cl_context &context, cl_kernel &kernel, cl_program &program)
{
	program = NULL;
	kernel = NULL;

	cl_int ret;

	FILE *fp;
	const char fileName[] = "kernel.cl";
	size_t source_size;
	char *source_str;


	fp = fopen(fileName, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char *)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	program = clCreateProgramWithSource(context, 1, (const char **)&source_str, (const size_t *)&source_size, &ret);
	ret = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
	kernel = clCreateKernel(program, "test", &ret);

	return ret;
}

cl_int run_kernel(cl_context &context, cl_command_queue &command_queue, cl_kernel &kernel)
{
	cl_mem res_arr = NULL;
	cl_mem rand_seed = NULL;
	cl_mem spheres = NULL;

	size_t mem_length = NPIX;
	cl_float4 *total = new cl_float4[mem_length];
	Sphere *s = Cornell_Box();
	cl_int *rs = new cl_int[mem_length];
	cl_int ret;

	for (int i = 0; i < mem_length; ++i)
		rs[i] = rand();

	res_arr = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_length * sizeof(cl_float4), NULL, &ret);
	ret = clEnqueueWriteBuffer(command_queue, res_arr, CL_TRUE, 0, mem_length * sizeof(cl_float4), total, 0, NULL, NULL);
	ret = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&res_arr);

	rand_seed = clCreateBuffer(context, CL_MEM_READ_WRITE, mem_length * sizeof(cl_int), NULL, &ret);
	ret = clEnqueueWriteBuffer(command_queue, rand_seed, CL_TRUE, 0, mem_length * sizeof(cl_int), rs, 0, NULL, NULL);
	ret = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&rand_seed);

	spheres = clCreateBuffer(context, CL_MEM_READ_WRITE, NOBJ * sizeof(Sphere), NULL, &ret);
	ret = clEnqueueWriteBuffer(command_queue, spheres, CL_TRUE, 0, NOBJ * sizeof(Sphere), s, 0, NULL, NULL);
	ret = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&spheres);

	size_t global_work_size[1] = { (size_t)mem_length };

	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, global_work_size, NULL, 0, NULL, NULL);
	ret = clEnqueueReadBuffer(command_queue, res_arr, CL_TRUE, 0, mem_length * sizeof(cl_float4), total, 0, NULL, NULL);


	// for (int i = 0; i < mem_length; ++i) {
	// 	std::cout << total[i].x << " " << total[i].y << " " << total[i].z << "\n";
	// }

	std::ofstream fout("super_img.ppm");
	fout << "P3\n" << WIDTH << " " << HEIGHT << "\n255\n";

	for (int i = NPIX - 1; i >= 0; --i) {
		fout << gamma_correction(total[i].x) << " "
			<< gamma_correction(total[i].y) << " "
			<< gamma_correction(total[i].z) << "\n";
	}

	fout.close();

	std::cout << "Done !!!\n";


	delete[] total;

	ret = clReleaseMemObject(res_arr);

	return ret;
}

cl_int free_kernel(cl_context &context, cl_command_queue &command_queue, cl_kernel &kernel, cl_program &program)
{
	cl_int ret;

	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);

	return ret;
}

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

int gamma_correction(float val)
{
	return int(255.0f * sqrt(ffmax(0.0f, ffmin(1.0f, val))));
}

Sphere *Cornell_Box()
{
	std::cout << "Initialize scene ...\n\n";

	Sphere *list = new Sphere[NOBJ];

	int i = 0;
	cl_float4 s1 = { -10277.5f,  0.0f,  0.0f  };
	cl_float4 s2 = {  10277.5f,  0.0f,  0.0f  };
	cl_float4 s3 = {  0.0f,  10277.5f,  0.0f  };
	cl_float4 s4 = {  0.0f, -10277.5f,  0.0f  };
	cl_float4 s5 = {  0.0f,  0.0f,  10555.0f  };
	cl_float4 s6 = {  0.0f,  337.5f,  277.5f  };
	cl_float4 s7 = {  140.5f, -187.5f, 50.0f  };
	cl_float4 s8 = { -40.5f, -167.5f, 250.0f };
	cl_float4 s9 = { -130.5f, -227.5f, 30.0f };

	cl_float4 gn = { 1.0f, 0.41f, 0.7f };
	cl_float4 rd = { 0.0f, 0.69f, 0.78f };
	cl_float4 bl = { 0.0f, 0.0f, 0.0f };
	cl_float4 gh = { 0.7f, 0.7f, 0.7f };
	cl_float4 aq = { 0.5f, 1.0f, 0.8f };
	cl_float4 super = { 15.0f, 15.0f, 15.0f };

	Material lwall = { gn, 0.0f, 0.0f, bl, 1.0f };
	Material rwall = { rd, 0.0f, 0.0f, bl, 1.0f };
	Material owall = { gh, 0.0f, 0.0f, bl, 1.0f };
	Material metal = { aq, 1.0f, 0.0f, bl, 1.0f };
	Material glass = { bl, 1.0f, 1.0f, bl, 1.5f };
	Material light = { bl, 0.0f, 0.0f, super, 1.0f };

	Sphere a1 = { s1, 10000.0f, lwall };
	Sphere a2 = { s2, 10000.0f, rwall };
	Sphere a3 = { s3, 10000.0f, owall };
	Sphere a4 = { s4, 10000.0f, owall };
	Sphere a5 = { s5, 10000.0f, owall };
	Sphere a6 = { s6, 80.0f, light };
	Sphere a7 = { s7, 90.0f, glass };
	Sphere a8 = { s8, 110.0f, metal };
	Sphere a9 = { s9, 50.0f, owall };

	list[i++] = a1;
	list[i++] = a2;
	list[i++] = a3;
	list[i++] = a4;
	list[i++] = a5;

	list[i++] = a6;
	list[i++] = a7;
	list[i++] = a8;
	list[i++] = a9;

	std::cout << "Number of objects: " << i << "\n\n";

	return list;
}
