// INCLUDES
#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

// PREPROCESSOR CONSTANTS
#define BUFFER 256
#define AXES 3
#define PARAMS 2
#define ARRAY_SIZE 2

enum Axis { X, Y, Z };
enum Params { VEL, ALT};

// STRUCTURES
struct no_internal_arrays
{
	int x;
	int y;
	int z;
	int vel;
	int alt;
};

struct with_fixed_internal_arrays
{
	char name[BUFFER];
	int x_y_z[AXES];
	int vel_alt[PARAMS];
};

// PROTOTYPES
void set_no_arrays_one(struct no_internal_arrays*);
void print_no_arrays_one(struct no_internal_arrays);

void set_arrays_one(struct no_internal_arrays[ARRAY_SIZE]);
void print_arrays_one(struct no_internal_arrays[ARRAY_SIZE]);

void set_dynamic_arrays_one(struct no_internal_arrays*, int);
void print_dynamic_arrays_one(struct no_internal_arrays*, int);

void set_no_structure_array_with_fixed_internal_arrays(struct with_fixed_internal_arrays*);
void print_no_structure_array_with_fixed_internal_arrays(struct with_fixed_internal_arrays);
void set_no_structure_array_with_fixed_internal_arrays_version_two(struct with_fixed_internal_arrays*);

void set_with_structure_array_with_fixed_internal_arrays(struct with_fixed_internal_arrays[ARRAY_SIZE]);
void print_with_structure_array_with_fixed_internal_arrays(struct with_fixed_internal_arrays[ARRAY_SIZE]);
void set_with_structure_array_with_fixed_internal_arrays_version_two(struct with_fixed_internal_arrays*);

// MAIN LOOP
int main(int argc, char* argv[])
{
	struct no_internal_arrays no_arrays_one;

	// When a structure has no arrays, or is not an array istelf, these are the valid ways of passing it to a function.

	set_no_arrays_one(&no_arrays_one); // Pass by reference (address). The structure member variables are being modified. 
	print_no_arrays_one(no_arrays_one); // Pass by value. The structure member variables do not need modification.

	// Look at the functions to clarify how the arguments must be formatted to accept the structure,
	// for each of these cases...
	//
	// Recieve by reference (pointer to structure).
	// void set_no_arrays_one(struct no_internal_arrays* no_arrs_one) 
	// 
	// In this case, access to the member variables of the pointed to structure must be handled 
	// inside the function with the ARROW OPERATOR (->), like this...
	// 
	// no_arrs_one->x
	// 
	// 
	// Recieve by value. No pointer. Fine if you do not need to modify the member variables.
	// void print_no_arrays_one(struct no_internal_arrays no_arrs_one) 
	//
	// If passed by value this way, the member variable must be accessed with the dot operator (.), like this...
	//
	// no_arrs_one.x
	//
	// That should now be clear, for once and all.
	// Now, onto arrays...

	struct no_internal_arrays arrays_one[ARRAY_SIZE];

	// There are several ways of passing arrays of structures to a function. 
	// In the first case, simply pass the array to the function.
	// An array is already a reference address by "nature". 
	// This works perfectly well, and the array is passed by reference, can be modified and accessed.

	set_arrays_one(arrays_one); 
	print_arrays_one(arrays_one);
	
	// What is the only drawback?
	// The SIZE OF THE ARRAY must already be known by the function before it will recieve the array it is being sent.
	//
	// void set_arrays_one(struct no_internal_arrays arrs_one[ARRAY_SIZE])
	//
	// It is handled inside the function like any other "conventional" array, there after...
	//
	// arrs_one[0].x
	//
	// This is fine, for arrays of structures that are not going to change size during the program execution lifetime.
	// But this is not always the case. If thesre is a possibility that an array might change size 
	// (made with malloc(), for example), then another, more flexible procedure will be required.
	// Let's do it...
	//
	// When dealing with dynamic memory, pointers are absolutely OBLIGATORY.
	// Also, the size of what you are going to make a dynamic array out of is extremely important,
	// or you risk memory leaks all over the place.
	// Here is the size of the basic structure, in bytes.
	// It is five ints, so the size will be 4 bytes * 5 members = 20 bytes
	printf("Size of structure no_internal_arrays: %d bytes\n\n", (int)sizeof(struct no_internal_arrays));

	int dynamic_array_size = 3; // Determine the size of the array (how many elements)
	struct no_internal_arrays* ptr_arrays_one = NULL; // Make a pointer of the structure, and point it to NULL.

	// Create the structure array during runtime with malloc().

	ptr_arrays_one = (struct no_internal_arrays*)malloc(sizeof(struct no_internal_arrays) * dynamic_array_size);
	// See? malloc() is great. Don't forget to typecast it, though, at definition time. It is a good idea.

	// Essentially, what we now have defined is ptr_arrays_one[3], of 60 bytes.
	// Now, let's see how to populate and display this.

	set_dynamic_arrays_one(ptr_arrays_one, dynamic_array_size);
	print_dynamic_arrays_one(ptr_arrays_one, dynamic_array_size);

	// The function recieves it as a pointer. This is important, as we are passing a pointer.
	//
	// void set_dynamic_arrays_one(struct no_internal_arrays* dyn_arrs_one, int array_size)
	// 
	// The size of the array should be passed as well, by value preferably, so that it is
	// not ever accidentally altered inside the function. Passing the size is necessary as,
	// because the array was created at run time, the size could vary and the function will not be
	// able to handle the dynamic array effectively in any loops that could be specified.
	// 
	// Inside the function, the arrow operator is preferably what should be used for access or modification...
	//
	// (dyn_arrs_one + i)->x
	//
	// ALWAYS free dynamic arrays after they are done with.
	free(ptr_arrays_one);

	// Now, onto a single element structure that contains internal, fixed size arrays.
	struct with_fixed_internal_arrays no_struct_array_internal_fixed_arrays_one;
	// This structure also contains a string, to make it interesting, which is noting more than an array of chars.
	
	// Note, we are back to passing it for mofification by reference with the & address, 
	// because it is a sigle structure variable, despite it having internal arrays.
	// These are accessed accordingly inside the function, as if they are normal arrays, after the
	// member element has been accessed with the arrow operator.
	// 
	// intern_fixed_arrs_one->x_y_z[X]

	printf("Size of structure with_internal_arrays: %d bytes\n\n", (int)sizeof(struct with_fixed_internal_arrays));

	set_no_structure_array_with_fixed_internal_arrays(&no_struct_array_internal_fixed_arrays_one);
	print_no_structure_array_with_fixed_internal_arrays(no_struct_array_internal_fixed_arrays_one);

	// For printing it, we can get away with simply passing the structure by value.
	// Inside the function, the value can be accessed like a normal structure member variable, with the dot operator.
	//
	// intern_fixed_arrs_one.x_y_z[X]
	//
	// BUT WAIT! There is yet another way of accessing for modification by reference, 
	// when we have internal arrays inside a structure, and are using pointers.
	// Enter POINTER ARITHMETIC...
	//
	// *((intern_fixed_arrs_one)->x_y_z + X)
	//
	// It is, effectively, the same as...
	//
	// intern_fixed_arrs_one->x_y_z[X]
	//
	// as used before.

	set_no_structure_array_with_fixed_internal_arrays_version_two(&no_struct_array_internal_fixed_arrays_one);
	print_no_structure_array_with_fixed_internal_arrays(no_struct_array_internal_fixed_arrays_one);

	// This can also be done without any adverse effects, and respects the fact we have passed a reference
	// (akin to a pointer) more effectively. In other words, when reading the code, it is more obvious
	// what is happening, when done this way.

	// Moving on. Now we have an array of a structure again, but this time with internal arrays.

	struct with_fixed_internal_arrays with_struct_array_internal_fixed_arrays_one[ARRAY_SIZE];

	set_with_structure_array_with_fixed_internal_arrays(with_struct_array_internal_fixed_arrays_one);
	print_with_structure_array_with_fixed_internal_arrays(with_struct_array_internal_fixed_arrays_one);

	set_with_structure_array_with_fixed_internal_arrays_version_two(with_struct_array_internal_fixed_arrays_one);
	print_with_structure_array_with_fixed_internal_arrays(with_struct_array_internal_fixed_arrays_one);

	_getch();

	return 0;
}


// FUNCTIONS
void set_no_arrays_one(struct no_internal_arrays* no_arrs_one)
{
	no_arrs_one->x = 10;
	no_arrs_one->y = 25;
	no_arrs_one->z = 45;
	no_arrs_one->vel = 60;
	no_arrs_one->alt = 15;
}

void print_no_arrays_one(struct no_internal_arrays no_arrs_one)
{
	printf("X = %d\n", no_arrs_one.x);
	printf("Y = %d\n", no_arrs_one.y);
	printf("Z = %d\n", no_arrs_one.z);
	printf("Vel = %d\n", no_arrs_one.vel);
	printf("Alt = %d\n\n", no_arrs_one.alt);
}

void set_arrays_one(struct no_internal_arrays arrs_one[ARRAY_SIZE])
{
	arrs_one[0].x = 100;
	arrs_one[0].y = 110;
	arrs_one[0].z = 120;
	arrs_one[0].vel = 130;
	arrs_one[0].alt = 140;

	arrs_one[1].x = 200;
	arrs_one[1].y = 210;
	arrs_one[1].z = 220;
	arrs_one[1].vel = 230;
	arrs_one[1].alt = 240;
}

void print_arrays_one(struct no_internal_arrays arrs_one[ARRAY_SIZE])
{
	printf("X0 = %d\n", arrs_one[0].x);
	printf("Y0 = %d\n", arrs_one[0].y);
	printf("Z0 = %d\n", arrs_one[0].z);
	printf("Vel0 = %d\n", arrs_one[0].vel);
	printf("Alt0 = %d\n\n", arrs_one[0].alt);

	printf("X1 = %d\n", arrs_one[1].x);
	printf("Y1 = %d\n", arrs_one[1].y);
	printf("Z1 = %d\n", arrs_one[1].z);
	printf("Vel1 = %d\n", arrs_one[1].vel);
	printf("Alt1 = %d\n\n", arrs_one[1].alt);
}

void set_dynamic_arrays_one(struct no_internal_arrays* dyn_arrs_one, int array_size)
{
	// The int array size is passed by value so that if a for loop is being used to
	// initialize the array, it knows how big the dynamic array is. We will use it...
	int i = 0;
	for (i = 0; i < array_size; i++)
	{
		//(dyn_arrs_one + i)->x = ((i + 1) * 10);
		dyn_arrs_one[i].x = ((i + 1) * 10); // Just pointing out that either way of modification can be used here.
		(dyn_arrs_one + i)->y = ((i + 1) * 20); // But the safer method is the use of the arrow operator,
		(dyn_arrs_one + i)->z = ((i + 1) * 30); // as we are manipulating a pointer, after all.
		(dyn_arrs_one + i)->vel = ((i + 1) * 40);
		(dyn_arrs_one + i)->alt = ((i + 1) * 50);
	}
}

void print_dynamic_arrays_one(struct no_internal_arrays* dyn_arrs_one, int array_size)
{
	int i = 0;
	for (i = 0; i < array_size; i++)
	{
		printf("Run %d\n", i);
		//printf("Y%d = %d\n", i, (dyn_arrs_one + i)->x);
		printf("X%d = %d\n", i, dyn_arrs_one[i].x); // Just pointing out that either way of access can be used here.
		printf("Y%d = %d\n", i, (dyn_arrs_one + i)->y); // But the safer method is the use of the arrow operator,
		printf("Z%d = %d\n", i, (dyn_arrs_one + i)->z); // as we are accessing a pointer, after all.
		printf("Vel%d = %d\n", i, (dyn_arrs_one + i)->vel);
		printf("Alt%d = %d\n\n", i, (dyn_arrs_one + i)->alt);

	}
}

void set_no_structure_array_with_fixed_internal_arrays(struct with_fixed_internal_arrays* intern_fixed_arrs_one)
{
	strcpy_s(intern_fixed_arrs_one->name, BUFFER, "The Name!");
	intern_fixed_arrs_one->x_y_z[X] = 300;
	intern_fixed_arrs_one->x_y_z[Y] = 200;
	intern_fixed_arrs_one->x_y_z[Z] = 100;
	intern_fixed_arrs_one->vel_alt[VEL] = 55;
	intern_fixed_arrs_one->vel_alt[ALT] = 1500;
}

void print_no_structure_array_with_fixed_internal_arrays(struct with_fixed_internal_arrays intern_fixed_arrs_one)
{
	printf("%s\n", intern_fixed_arrs_one.name);
	printf("X = %d\n", intern_fixed_arrs_one.x_y_z[X]);
	printf("Y = %d\n", intern_fixed_arrs_one.x_y_z[Y]);
	printf("Z = %d\n", intern_fixed_arrs_one.x_y_z[Z]);
	printf("X = %d\n", intern_fixed_arrs_one.vel_alt[VEL]);
	printf("X = %d\n\n", intern_fixed_arrs_one.vel_alt[ALT]);
}

void set_no_structure_array_with_fixed_internal_arrays_version_two(
	struct with_fixed_internal_arrays* intern_fixed_arrs_one)
{
	strcpy_s(intern_fixed_arrs_one->name, BUFFER, "The New Name!");
	*((intern_fixed_arrs_one)->x_y_z + X) = 500;
	*((intern_fixed_arrs_one)->x_y_z + Y) = 400;
	*((intern_fixed_arrs_one)->x_y_z + Z) = 300;
	*((intern_fixed_arrs_one)->vel_alt + VEL) = 155;
	*((intern_fixed_arrs_one)->vel_alt + ALT) = 3500;
}

void set_with_structure_array_with_fixed_internal_arrays(
	struct with_fixed_internal_arrays struct_array_with_fixed_intern_array[ARRAY_SIZE])
{
	int i;
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		strcpy_s(struct_array_with_fixed_intern_array[i].name, BUFFER, "The Newest Name!");
		struct_array_with_fixed_intern_array[i].x_y_z[X] = (i * 10) + (100);
		struct_array_with_fixed_intern_array[i].x_y_z[Y] = (i * 10) + (200);
		struct_array_with_fixed_intern_array[i].x_y_z[Z] = (i * 10) + (300);
		struct_array_with_fixed_intern_array[i].vel_alt[VEL] = (i * 5) + (25);
		struct_array_with_fixed_intern_array[i].vel_alt[ALT] = (i * 5) + (50);
	}
}

void print_with_structure_array_with_fixed_internal_arrays(
	struct with_fixed_internal_arrays struct_array_with_fixed_intern_array[ARRAY_SIZE])
{
	int i;
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		printf("%s\n", struct_array_with_fixed_intern_array[i].name);
		printf("X%d = %d\n", i, struct_array_with_fixed_intern_array[i].x_y_z[X]);
		printf("Y%d = %d\n", i, struct_array_with_fixed_intern_array[i].x_y_z[Y]);
		printf("Z%d = %d\n", i, struct_array_with_fixed_intern_array[i].x_y_z[Z]);
		printf("VEL%d = %d\n", i, struct_array_with_fixed_intern_array[i].vel_alt[VEL]);
		printf("ALT%d = %d\n\n", i, struct_array_with_fixed_intern_array[i].vel_alt[ALT]);
	}
}

void set_with_structure_array_with_fixed_internal_arrays_version_two(
	struct with_fixed_internal_arrays* struct_array_with_fixed_intern_array)
{
	int i;
	for (i = 0; i < ARRAY_SIZE; i++)
	{
		strcpy_s((struct_array_with_fixed_intern_array+i)->name, BUFFER, "The Newest, Newest Name!");
		*((struct_array_with_fixed_intern_array + i)->x_y_z + X) = (i * 10) + (500);
		*((struct_array_with_fixed_intern_array + i)->x_y_z + Y) = (i * 10) + (600);
		*((struct_array_with_fixed_intern_array + i)->x_y_z + Z) = (i * 10) + (700);
		*((struct_array_with_fixed_intern_array + i)->vel_alt + VEL) = (i * 5) + (125);
		*((struct_array_with_fixed_intern_array + i)->vel_alt + ALT) = (i * 5) + (150);
	}
}

