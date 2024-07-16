/*
 * File    : sfun_fuzzyutilsug.h
 */
 

#include <math.h>

#define MAX(A,B)  ((A) > (B) ? (A) : (B))
#define MIN(A,B)  ((A) < (B) ? (A) : (B))
#define PI 3.14159265359
#define Merror 1.0
#define Mder 1.0
#define Mu 1.0

typedef struct in_mem {
  double *width;         /* width of each input membership function..  */
  double *center;       /* Center of each input membership function.                */
  double *dom;          /* Degree of membership for each membership function.       */
} IN_MEM;

typedef struct out_mem {
  double *center;       /* Center of each output membership function.               */
} OUT_MEM;

typedef struct fuz_sys {
  IN_MEM  *emem;        /* Groups all fuzzy system parameters in a single variable. */
  IN_MEM  *edotmem;
  OUT_MEM *outmem;
  int Rules[3][3];
  double Premisas[3][3];
} FUZ_SYS;


void fuzzy_init(FUZ_SYS *fuzzy_system);
void fuzzy_free(FUZ_SYS *fuzzy_system);
double fuzzy_control(double e, double edot, FUZ_SYS *fuzzy_system);
void fuzzyify(double u, IN_MEM *mem);
double leftall(double u, double w, double c);
double rightall(double u, double w, double c);
double triangle(double u, double w, double c);
void match(const IN_MEM *emem, const IN_MEM *edotmem, int *pos);
double inf_defuzz(IN_MEM *emem, IN_MEM *edotmem, OUT_MEM *outmem, int *pos);



void fuzzy_init(FUZ_SYS *fuzzy_system) {

/* Define the input and output membership functions. */  

  int i;

  /* Allocate memory for membership functions. */
  if (!(fuzzy_system->emem = (IN_MEM *) malloc(sizeof(IN_MEM)))) {
     exit(1);
  }
  if (!(fuzzy_system->edotmem = (IN_MEM *) malloc(sizeof(IN_MEM)))) {
     exit(1);
  }
  if (!(fuzzy_system->outmem = (OUT_MEM *) malloc(sizeof(OUT_MEM)))) {
    exit(1);
  }

  
  if (!(fuzzy_system->emem->width = (double *) malloc(5*sizeof(double)))) {
    exit(1);
  }
  if (!(fuzzy_system->emem->center = (double *) malloc(5*sizeof(double)))) {
    exit(1);
  }
  if (!(fuzzy_system->emem->dom = (double *) malloc(5*sizeof(double)))) {
    exit(1);
  }

  
  if (!(fuzzy_system->edotmem->width = (double *) malloc(5*sizeof(double)))) {
    exit(1);
  }
  if (!(fuzzy_system->edotmem->center = (double *) malloc(5*sizeof(double)))) {
    exit(1);
  }
  if (!(fuzzy_system->edotmem->dom = (double *) malloc(5*sizeof(double)))) {
    exit(1);
  }

  if (!(fuzzy_system->outmem->center = (double *) malloc(7*sizeof(double)))) {
     exit(1);
  }

  /* Initialize  */
  fuzzy_system->emem->width[0] = 1.0*Merror;  
  fuzzy_system->emem->width[1] = 1.1*Merror;  
  fuzzy_system->emem->width[2] = 1.0*Merror;    
  
  fuzzy_system->emem->center[0] = -0.75*Merror;  
  fuzzy_system->emem->center[1] = 0.0*Merror;  
  fuzzy_system->emem->center[2] = 0.75*Merror; 
  
  fuzzy_system->edotmem->width[0] = 1.0*Mder;
  fuzzy_system->edotmem->width[1] = 1.1*Mder;
  fuzzy_system->edotmem->width[2] = 1.0*Mder;

  fuzzy_system->edotmem->center[0] = -0.75*Mder;
  fuzzy_system->edotmem->center[1] = 0.0*Mder;
  fuzzy_system->edotmem->center[2] = 0.75*Mder;

  fuzzy_system->outmem->center[0] = 0.0*Mu;
  fuzzy_system->outmem->center[1] = 0.5*Mu;
  fuzzy_system->outmem->center[2] = 1.0*Mu;
  
  fuzzy_system->Rules[0][0]=0.0;
  fuzzy_system->Rules[0][1]=0.0;
  fuzzy_system->Rules[0][2]=0.0;
  
  fuzzy_system->Rules[1][0]=1.0;
  fuzzy_system->Rules[1][1]=0.5;
  fuzzy_system->Rules[1][2]=0.0;
  
  fuzzy_system->Rules[2][0]=1;
  fuzzy_system->Rules[2][1]=1;
  fuzzy_system->Rules[2][2]=1;
}

/************************************************************************************/
void fuzzy_free(FUZ_SYS *fuzzy_system) {

/* Free memory allocated in fuzzy_init(). */ 

  free(fuzzy_system->emem->width);
  free(fuzzy_system->emem->center);
  free(fuzzy_system->emem->dom);
  
  free(fuzzy_system->edotmem->width);
  free(fuzzy_system->edotmem->center);
  free(fuzzy_system->edotmem->dom);

  free(fuzzy_system->outmem->center);
  free(fuzzy_system->emem);
  free(fuzzy_system->edotmem);
  free(fuzzy_system->outmem);
}


/************************************************************************************/

double fuzzy_control(double e, double edot, FUZ_SYS *fuzzy_system) {
  
/* Given crisp inputs e and edot, determine the crisp output u. */

  double per_error;
  double per_der_error;  
  double Num_Con;
  double Den_Con;
  int i;
  int j;
  
  
  fuzzyify(e, fuzzy_system->emem);
  fuzzyify(edot, fuzzy_system->edotmem);
  
 for (i=0; i<5; i++) {
   for (j=0; j<5; j++) {
     per_error=fuzzy_system->emem->dom[i];
     per_der_error=fuzzy_system->edotmem->dom[j];
     fuzzy_system->Premisas[i][j]=MIN(per_error,per_der_error);
   }
 }
 
Num_Con=0;
Den_Con=0;
 for (i=0; i<5; i++) {
   for (j=0; j<5; j++) {
     Num_Con=Num_Con + (fuzzy_system->Premisas[i][j]*fuzzy_system->outmem->center[ fuzzy_system->Rules[i][j] ]);
     Den_Con=Den_Con+fuzzy_system->Premisas[i][j];  
   }
 }

return Num_Con/Den_Con;
  
 /*match(fuzzy_system->emem, fuzzy_system->edotmem, pos);*/
/*  return inf_defuzz(fuzzy_system->emem, fuzzy_system->edotmem, fuzzy_system->outmem, pos); */

}

/************************************************************************************/

void fuzzyify(double u, IN_MEM *mem) {

/* Fuzzify the input u by determining the degree of membership for each membership
   function in mem. Assumes 5 membership functions, with first and last membership
   functions leftall and rightall respectively.  Other membership functions are
   triangular. */
 
  int i;

  mem->dom[0] = leftall(u, mem->width[0], mem->center[0]);
  for (i=1; i<4; i++) 
    mem->dom[i] = triangle(u, mem->width[i], mem->center[i]);
  mem->dom[4] = rightall(u, mem->width[4], mem->center[4]);
}

/************************************************************************************/

double leftall(double u, double w, double c)

/* Determine degree of membership for a leftall membership function.
   NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
  if (u < c)
    return 1.0;
  else
    return MAX(0,(1-(u-c)/w));
} 

/************************************************************************************/

double rightall(double u, double w, double c)
/* Determine degree of membership for a RIGHTALL membership function
   NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
  if (u >= c)
         return 1.0;
  else
         return MAX(0,(1-(c-u)/w));
}

/************************************************************************************/

double triangle(double u, double w, double c)

/* Determine degree of membership for a TRIANGLE membership function
   NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
  if (u >= c)
    return MAX(0,(1-(u-c)/w));
  else
    return MAX(0,(1-(c-u)/w));
}

/************************************************************************************/

void match(const IN_MEM *emem, const IN_MEM *edotmem, int *pos) {

/* For each universe of discourse, determine the index of the first membership function
   with a non-zero degree (i.e. match the rules to the current inputs to find which rules 
   are on).  These indices are used to determine which four rules to evaluate.  (NOTE: 
   A 2 input sytem with no more than 50% overlap for input membership functions only
   requires the evaluation of at most 4 rules.) */ 
  
  int i;

  for (i=0; i<5; i++) {
    if(emem->dom[i] != 0) {
      pos[0] = i;
      break;
    }
  }
  for (i=0; i<5; i++) {
    if(edotmem->dom[i] != 0) {
      pos[1] = i;
      break;
    }
  }
}

/************************************************************************************/

double inf_defuzz(IN_MEM *emem, IN_MEM *edotmem, OUT_MEM *outmem, int *pos) {

/* We use the degrees of membership found in the function match() to form the implied
   fuzzy sets. The correct output membership function for each rule is determined by
   adding (and saturating) a shifted version of the input membership function indices
   (this implements the typical pattern of linguistic-numeric indices in the body of 
   the table of rules).  In this way we compute the rule-base at every step, rather
   than storing the rule-base in a table.  Defuzzification is also performed using
   the center-of-gravity method.  A crisp output is returned. */

  double outdom, area, Atot = 0, WAtot = 0;
  int i, j, out_index;

  for(i=0; i<2; i++) {
    for(j=0; j<2; j++) {
      if ( ((pos[0]+i)<5) && ((pos[1]+j)<5)) { /* Check that bounds are not exceeded. */
        outdom = 0;

        /* Shift indices left. */
        out_index = ((pos[0]+i)-2) + ((pos[1]+j)-2); 

        /* Saturate */
        if (out_index < -2)
          out_index = -2;
        else if (out_index > 2)
          out_index = 2;

        /* Shift indices right.*/
        out_index += 2;

        /* Determine the certainty of the premise */
        outdom = MIN((emem->dom[pos[0]+i]), (edotmem->dom[pos[1]+j]));

        /* Defuzzify */
     /*   area = 2*outmem->width*(outdom - (outdom*outdom)/2);*/
        Atot += area;
        WAtot += area*outmem->center[out_index];
      }
    }
  }
  /* Return the crisp value.  Minus sign required to give correct output for 
     pendulum system!  Note that this minus sign actually ensures that the table of
     indices works out as shown in class. */

  return -(WAtot/Atot); 
}