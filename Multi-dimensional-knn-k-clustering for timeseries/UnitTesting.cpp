#include <CUnit/Basic.h>
#include "functions.h"
#include "data_handling.h"
#include <iostream>
#include <random>
#include <algorithm>
//g++ -o test UnitTesting.cpp functions.cpp data_handling.cpp -lcunit

using namespace std;

static FILE* temp_file = NULL;

int init_suite1(void)
{
   if (NULL == (temp_file = fopen("temp.txt", "w+"))) {
      return -1;
   }
   else {
      return 0;
   }
}

int clean_suite1(void)
{
   if (0 != fclose(temp_file)) {
      return -1;
   }
   else {
      temp_file = NULL;
      return 0;
   }
}

void testMOD(){
   cout<<"Testing mod...\n";
    CU_ASSERT(0==mod(0.0,3.0));
}


void testDiscreteFrechet(){
   vector<pointType> points = get_data("nasdaq2017_LQ.csv");
   cout<<"Testing Discete Frechet...\n";
   CU_ASSERT(0==discreteFrechet(points[0],points[0]));
}

void testSnapping2D(){
   vector<pointType> points = get_data("nasdaq2017_LQ.csv");
   add_x_values(points);
   cout<<"Testing 2D Snapping...\n";
   uniform_real_distribution<double> uni_d(0.0, 1);
   vector<pointType> grid_curve_query(1);

   double** t_value;
   random_device r;
   default_random_engine gen(r());
   int initialSize=points[0].coords.size();
   t_value = new double*[4];
    for (int i = 0; i < 4; i++){
        t_value[i] = new double[initialSize];
        for (int j = 0; j < initialSize; j++){
            t_value[i][j] = uni_d(gen);
        }
    }
    
   grid_curve_query[0] = snapping2D(points[0], 1, t_value[0]);
   /*std::vector<pointType>* grid_curves;
    grid_curves = new vector<pointType>[4];
    for (int i = 0; i < 4; i++){
        grid_curves[i].resize(points.size());
        for (int j = 0; j < points.size(); j++){
            grid_curves[i][j] = snapping2D(points[j], 10, t_value[i]);
        }

    }*/
   int newSize=grid_curve_query[0].coords.size();
   cout<<newSize<<endl<<initialSize<<endl;
   CU_ASSERT(initialSize>newSize);
}


void testFiltering(){
   vector<pointType> points = get_data("nasdaq2017_LQ.csv");
   cout<<"Testing filtering...\n";
   pointType filtered=filtering(points[0],0.2);
   cout<<points[0].coords.size()<<endl<<filtered.coords.size()<<endl;
   CU_ASSERT(points[0].coords.size()>filtered.coords.size());
}


int main(){
    CU_pSuite pSuite = NULL;
    /* initialize the CUnit test registry */
   if (CUE_SUCCESS != CU_initialize_registry())
      return CU_get_error();

   /* add a suite to the registry */
   pSuite = CU_add_suite("Suite_1", init_suite1, clean_suite1);
   if (NULL == pSuite) {
      CU_cleanup_registry();
      return CU_get_error();
   }
    if (NULL == CU_add_test(pSuite, "test of fprintf()", testMOD))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if (NULL == CU_add_test(pSuite, "test of frechet()", testDiscreteFrechet))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if (NULL == CU_add_test(pSuite, "test of 2DSnapping()", testSnapping2D))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }

   if (NULL == CU_add_test(pSuite, "test of filtering()", testFiltering))
   {
      CU_cleanup_registry();
      return CU_get_error();
   }


   CU_basic_set_mode(CU_BRM_VERBOSE);
   CU_basic_run_tests();
   CU_cleanup_registry();
   remove("temp.txt");
   return CU_get_error();

    

    return 0;

}