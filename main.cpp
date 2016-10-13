#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <type_traits>
#include <stdint.h>
#include <windows.h>

#include "testTemplate.h"
//#include <sys/time.h>
//#include <sys/stat.h>
//#include <sys/types.h>

using namespace std;

int main()
{
    //=========================================================================
    // the size of this class would be 16 bytes
    // the highest size of member data type is 8 byte.(double) Therefore,
    // the alignment should be 8-byte; the address of this struct is divisible
    // evenly by 8.
    // What if additional char variable is added as a last member variable?
    // What size of this class would be ? (Answer is 24 bytes, next multiple of 8.)
    class C1
    {
    public:
        char   m1;      // 1-byte element
                        // 3-bytes of padding are placed here in order to make this class evenly divisible by 8
        int    m2;      // 4-byte element
        double m3;      // 8-byte element
        //char m4;        // add aditional 1-byte element (what size of this class is ?)
    };

    struct S1
    {
        char   m1;      // 1-byte element
                        // 3-byte padding are placed here
        int    m2;      // 4-byte element
        double m3;      // 8-byte element
    };

    C1 c1;
    S1 s1;
    C1 c1a[3];
    S1 s1a[3];

    // print alignment and the address of each member variable in the class
    printf("===== Structure Member Data Alignment =====\n");
    printf("class size  : %d bytes\n", sizeof(c1));
    printf("class align : %d bytes\n", __alignof(C1)); // MSVC specific, arg must be data type
    printf("class char  : %p\n", &c1.m1);
    printf("class int   : %p (+%d)\n", &c1.m2, ((char*)&c1.m2 - (char*)&c1.m1));
    printf("class double: %p (+%d)\n", &c1.m3, ((char*)&c1.m3 - (char*)&c1.m2));
    //printf("class char2 : %p (+%d)\n", &c1.m4, ((char*)&c1.m4 - (char*)&c1.m3));
    printf("\n");

    // print alignment and address of each member variable in the struct
    printf("struct size  : %d bytes\n", sizeof(s1));
    printf("struct align : %d bytes\n", __alignof(S1)); // MSVC specific, arg must be data type
    printf("struct char  : %p\n", &s1.m1);
    printf("struct int   : %p (+%d)\n", &s1.m2, ((char*)&s1.m2 - (char*)&s1.m1));
    printf("struct double: %p (+%d)\n", &s1.m3, ((char*)&s1.m3 - (char*)&s1.m2));
    printf("\n");

    // print the address for each element of array of class
    printf("class array [0]: %p\n", &c1a[0]);
    printf("class array [1]: %p (+%d)\n", &c1a[1], ((char*)&c1a[1] - (char*)&c1a[0]));
    printf("class array [2]: %p (+%d)\n", &c1a[2], ((char*)&c1a[2] - (char*)&c1a[1]));
    printf("\n");

    // print the address for each element of array of struct
    printf("struct array [0]: %p\n", &s1a[0]);
    printf("struct array [1]: %p (+%d)\n", &s1a[1], ((char*)&s1a[1] - (char*)&s1a[0]));
    printf("struct array [2]: %p (+%d)\n", &s1a[2], ((char*)&s1a[1] - (char*)&s1a[0]));
    printf("\n");


    //=========================================================================
    char c;
    short s;
    int i;
    long l;
    float f;
    double d;

    char ca[3];
    short sa[3];
    int ia[3];
    long la[3];
    float fa[3];
    double da[3];

    // print addresses of each data type (not need to be aligned) 
    printf("===== Normal Data Types (No Align Needed) =====\n");
    printf("char  : %p\n", &c);
    printf("short : %p\n", &s);
    printf("int   : %p\n", &i);
    printf("long  : %p\n", &l);
    printf("float : %p\n", &f);
    printf("double: %p\n", &d);
    printf("\n");

    // print addresses of array elements
    // Array element takes exact data size without padding, for example,
    // char array allocates 1 byte per element
    printf("char array [0]: %p\n", &ca[0]);
    printf("char array [1]: %p (+%d)\n", &ca[1], ((char*)&ca[1] - (char*)&ca[0]));
    printf("\n");

    printf("short array [0]: %p\n", &sa[0]);
    printf("short array [1]: %p (+%d)\n", &sa[1], ((char*)&sa[1] - (char*)&sa[0]));
    printf("\n");

    printf("int array [0]: %p\n", &ia[0]);
    printf("int array [1]: %p (+%d)\n", &ia[1], ((char*)&ia[1] - (char*)&ia[0]));
    printf("\n");

    printf("long array [0]: %p\n", &la[0]);
    printf("long array [1]: %p (+%d)\n", &la[1], ((char*)&la[1] - (char*)&la[0]));
    printf("\n");

    printf("float array [0]: %p\n", &fa[0]);
    printf("float array [1]: %p (+%d)\n", &fa[1], ((char*)&fa[1] - (char*)&fa[0]));
    printf("\n");

    printf("double array [0]: %p\n", &da[0]);
    printf("double array [1]: %p (+%d)\n", &da[1], ((char*)&da[1] - (char*)&da[0]));
    printf("\n");


    //=========================================================================
    // data alignment for SSE using __declspec(align(16)) (MSVC specific)
    __declspec(align(16)) float alignf = 1.0f;  // aligned single float
    __declspec(align(16)) float alignfa[100];   // aligned float array
    struct __declspec(align(16)) Sa             // aligned struct
    {
        float f[4];
    };
    Sa aligns;

    // print address of aligned data
    // the least significant digit should be 0
    printf("===== Alignment for SSE using __declspec(align(16)) =====\n");
    printf("aligned float : %p\n", &alignf);
    printf("aligned array : %p\n", alignfa);
    printf("aligned struct: %p\n", &aligns);
    printf("\n");


    //=========================================================================
    // manual data alignment with bit operator
    // allocate 15 byte larger array because in worst case, the data can be
    // misaligned upto 15 bytes.
    float* array = (float*)malloc(101 * sizeof(float) + 15); // 15 bytes bigger
    float* alignedArray = (float*)(((unsigned long)array + 15) & (~0x0F));

    printf("===== Manual Alignment for SSE using Bit Operator =====\n"); 
    printf("before align: %p\n", array);
    printf("after align : %p\n", alignedArray); // last digit should be 0
    printf("\n");

    free(array);
    array = alignedArray = 0;



	int *p1 = (int*)malloc(10 * sizeof *p1);
	printf("default-aligned addr:   %p\n", (void*)p1);
	free(p1);

	/*int *p2 = (int*)_aligned_malloc(1024, 10 * sizeof *p2);
	printf("1024-byte aligned addr: %p\n", (void*)p2);
	//free(p2);
	_aligned_free(p2);*/

	/*-----------------------------------------------------------*/
	// Note alignment should be 2^N where N is any positive int.
	size_t alignment = 16;

	// Using _aligned_malloc
	void* ptr = _aligned_malloc(100, alignment);
	if (ptr == NULL)
	{
		printf_s("Error allocation aligned memory.");
		return -1;
	}
	if (((unsigned long long)ptr % alignment) == 0)
		printf_s("This pointer, %p, is aligned on %zu\n",
			ptr, alignment);



	/***********************************************************/
	struct A {  // non-POD type
		int avg;
		A(int a, int b) : avg((a + b) / 2) {}
	};

	typedef std::aligned_storage<sizeof(A), alignof(A)>::type A_pod;

	A_pod a, b;
	new (&a) A(10, 20);
	b = a;
	std::cout << "aligned_storage: " << reinterpret_cast<A&>(b).avg << std::endl;

	/*************************************************************/


	/**************************************************************************************************/

	//  g++ -O2 -o test test.cpp

	runtest<int>();
	cout << endl;
	runtest<long>();
	cout << endl;
	runtest<long long>();
	cout << endl;

	/******************************************************************************************/

    system("pause");
	return 0;
}

