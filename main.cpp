#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>

#include <math.h> 

#include "duktape.h"

#define duk_register(ctx, jsName, functionName, argsCount)  \
	duk_push_c_function(ctx, functionName, argsCount);       \
	duk_put_global_string(ctx, jsName);

const double PI = std::atan(1.0)*4;


static double degToRad(double degrees) {
	return degrees * PI / 180;
}

static double getArrayValue(duk_context *ctx, int arrayID, int index) {
	duk_get_prop_index(ctx, arrayID, index);
	double value = duk_to_number(ctx, -1);
	duk_pop(ctx);
	return value;
}

static void returnArray(duk_context *ctx, double *data, int length) {
	duk_idx_t arr_idx = duk_push_array(ctx);
	int i;
	for (i = 0; i < length; i++) {
		duk_push_number(ctx, data[i]);
		duk_put_prop_index(ctx, arr_idx, i);
	}

	return;
}

static void hamiltonProduct(double *q1, double *q2, double *result) {
	double x = q1[3]*q2[0] + q1[0]*q2[3] + q1[1]*q2[2] - q1[2]*q2[1],
		   y = q1[3]*q2[1] - q1[0]*q2[2] + q1[1]*q2[3] + q1[2]*q2[0],
		   z = q1[3]*q2[2] + q1[0]*q2[1] - q1[1]*q2[0] + q1[2]*q2[3],
		   w = q1[3]*q2[3] - q1[0]*q2[0] - q1[1]*q2[1] - q1[2]*q2[2];

	result[0] = x;
	result[1] = y;
	result[2] = z;
	result[3] = w;

	return;
}

static duk_ret_t native_hamiltonProduct(duk_context *ctx) {
	int n = duk_get_top(ctx);
	// std::cout << "III " << n << duk_get_length(ctx, 0);

	int i;
	double *q1 = new double[4];
	double *q2 = new double[4];

	for (i = 0; i < 4; i++) {
		q1[i] = getArrayValue(ctx, 0, i);
		q2[i] = getArrayValue(ctx, 1, i);
	}


	double *result = new double[4];

	hamiltonProduct(q1, q2, result);

	returnArray(ctx, result, 4);

	return 1;
}

static void getQuatFromAxisAngle(double x, double y, double z, double angle, double *result) {
	double vectorLength = sqrt(x*x+y*y+z*z);

	if (vectorLength != 0) {
		x /= vectorLength;
		y /= vectorLength;
		z /= vectorLength;
	}

	double s = sin(degToRad(angle)*0.5);

	result[0] = x*s;
	result[1] = y*s;
	result[2] = z*s;
	result[3] = cos(degToRad(angle)*0.5);

	return;
}

static duk_ret_t native_getQuatFromAxisAngle(duk_context *ctx) {
	int n = duk_get_top(ctx);  /* #args */

	double x = duk_to_number(ctx, 0),
		   y = duk_to_number(ctx, 1),
		   z = duk_to_number(ctx, 2),
		   angle = duk_to_number(ctx, 3);


	double *result = new double[4];

	getQuatFromAxisAngle(x, y, z, angle, result);

	returnArray(ctx, result, 4);

	return 1;
}

static void rotate3d(double x, double y, double z, double xa, double ya, double za, double angle, double *result) {
	double *quaternion = new double[4];
	getQuatFromAxisAngle(xa,ya,za, angle, quaternion);

	double *quaternionR = new double[4];
	getQuatFromAxisAngle(-xa,-ya,-za, angle, quaternionR);

	double *resultTemp = new double[4];
	double dotData[4] = {x,y,z,0};
	hamiltonProduct(quaternion, dotData, resultTemp);

	double *resultFinal = new double[4];
	hamiltonProduct(resultTemp, quaternionR, resultFinal);

	result[0] = resultFinal[0];
	result[1] = resultFinal[1];
	result[2] = resultFinal[2];

	return;
}

static duk_ret_t native_rotate3d(duk_context *ctx) {
	int n = duk_get_top(ctx);  /* #args */

	double x = duk_to_number(ctx, 0),
		   y = duk_to_number(ctx, 1),
		   z = duk_to_number(ctx, 2),
		   xa = duk_to_number(ctx, 3),
		   ya = duk_to_number(ctx, 4),
		   za = duk_to_number(ctx, 5),
		   angle = duk_to_number(ctx, 6);

	double *result = new double[3];

	rotate3d(x, y, z, xa, ya, za, angle, result);

	returnArray(ctx, result, 3);

	return 1;
}

static duk_ret_t native_print(duk_context *ctx) {
	printf("%s\n", duk_to_string(ctx, 0));
	return 0;  /* no return value (= undefined) */
}

int main(int argc, char *argv[]) {
	std::ifstream infile("script.js");

	std::string str;
	std::string file_contents;
	while (std::getline(infile, str)) {
		file_contents += str;
		file_contents.push_back('\n');
	}

	// std::cout << file_contents;

	duk_context *ctx = duk_create_heap_default();

	duk_register(ctx, "print", native_print, 1);
	duk_register(ctx, "hamiltonProduct", native_hamiltonProduct, 2);
	duk_register(ctx, "getQuatFromAxisAngle", native_getQuatFromAxisAngle, 4);
	duk_register(ctx, "rotate3d", native_rotate3d, 7);


	std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
	int result = duk_peval_string(ctx, file_contents.c_str());
	std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
	std::cout << "Evaluating script.js took "
              << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
              << "microseconds.\n";

	if (result != 0) {
		/* Use duk_safe_to_string() to convert error into string.  This API
		 * call is guaranteed not to throw an error during the coercion.
		 */
		printf("Script error: %s\n", duk_safe_to_string(ctx, -1));
	}
	else {
		printf("Success result: %s\n", duk_safe_to_string(ctx, -1));
	}

	duk_destroy_heap(ctx);

	return 0;
}