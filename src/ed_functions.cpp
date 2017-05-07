/***********************************************************************/
/************************* DISCLAIMER **********************************/
/***********************************************************************/
/** This UCR Suite software is copyright protected   2012 by          **/
/** Thanawin Rakthanmanon, Bilson Campana, Abdullah Mueen,            **/
/** Gustavo Batista and Eamonn Keogh.                                 **/
/**                                                                   **/
/** Unless stated otherwise, all software is provided free of charge. **/
/** As well, all software is provided on an "as is" basis without     **/
/** warranty of any kind, express or implied. Under no circumstances  **/
/** and under no legal theory, whether in tort, contract,or otherwise,**/
/** shall Thanawin Rakthanmanon, Bilson Campana, Abdullah Mueen,      **/
/** Gustavo Batista, or Eamonn Keogh be liable to you or to any other **/
/** person for any indirect, special, incidental, or consequential    **/
/** damages of any character including, without limitation, damages   **/
/** for loss of goodwill, work stoppage, computer failure or          **/
/** malfunction, or for any and all other damages or losses.          **/
/**                                                                   **/
/** If you do not agree with these terms, then you you are advised to **/
/** not use this software.                                            **/
/***********************************************************************/
/***********************************************************************/

// [[Rcpp::interfaces(r, cpp)]]

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>
#include <Rcpp.h>
using namespace Rcpp;


#define INF 1e20       //Pseudo Infitinte number for this code

using namespace std;

/// Data structure for sorting the query.
typedef struct Index
{   double value;
  int    index;
} Index;


/// Comparison function for sorting the query.
/// The query will be sorted by absolute z-normalization value, |z_norm(Q[i])| from high to low.
int comp_ed(const void *a, const void* b)
{   Index* x = (Index*)a;
  Index* y = (Index*)b;
  return abs(y->value) - abs(x->value);
}


/// Main function for calculating ED distance between the query, Q, and current data, T.
/// Note that Q is already sorted by absolute z-normalization value, |z_norm(Q[i])|
//double distance(const double * const Q, const double * const T , const int& j , const int& m , const double& mean , const double& std , const int* const order, const double& bsf)
double distance(double * Q, double * T , const int& j , const int& m , const double& mean , const double& std , int* order, const double& bsf)
{
  int i;
  double sum = 0;
  for ( i = 0 ; i < m && sum < bsf ; i++ )
  {
    double x = (T[(order[i]+j)]-mean)/std;
    sum += (x-Q[i])*(x-Q[i]);
  }
  return sum;
}


/// If serious error happens, terminate the program.
void error_ed(int id)
{
  if(id==1)
    stop("ERROR : Memory can't be allocated!!!\n\n");
  else if ( id == 2 )
    stop("ERROR : File not Found!!!\n\n");
}



//' UCR ED Algorithm file-file method
//'
//' Sliding-window similarity search using euclidean distance. This implementation is very close to the UCR Suite command line utility, in that it takes files as inputs for both query and data
//'
//' @name ucred_ff
//' @param data character; path to data file
//' @param query character; path to query file
//' @param qlength integer; length of query (n data points). Usually the length of the data contained in \code{query}, but it can be shorter.
//' @return a ucred object. A list with the following elements
//' \itemize{
//'   \item \strong{location:} The starting location of the nearest neighbor of the given query, of size \code{qlength}, in the data. Note that location starts from 1.
//'   \item \strong{distance:} The euclidean distance between the nearest neighbor and the query.
//' }
//' @examples
//' #locate example data file
//' dataf <- system.file("extdata/col_sc.txt", package="rucrdtw")
//' #locate example query file
//' queryf <- system.file("extdata/mid_sc.txt", package="rucrdtw")
//' #determine length of query file
//' qlength <- length(scan(queryf))
//' #run query
//' ucred_ff(dataf, queryf, qlength)
//' @useDynLib rucrdtw, .registration=TRUE
//' @importFrom Rcpp sourceCpp
//' @export
// [[Rcpp::export]]
Rcpp::List ucred_ff(const char * data , const char * query, int qlength)
{
  FILE *fp;              // the input file pointer
  FILE *qp;              // the query file pointer

  double *Q;             // query array
  double *T;             // array of current data
  int *order;            // ordering of query by |z(q_i)|
  double bsf;            // best-so-far
  int m;                 // length of query
  long long loc = 0;     // answer: location of the best-so-far match

  double d;
  long long i , j ;
  double ex , ex2 , mean, std;

  //double t1,t2;

  //t1 = clock();

  bsf = INF;
  i = 0;
  j = 0;
  ex = ex2 = 0;

  fp = fopen(data,"r");
  if( fp == NULL )
    error_ed(2);

  qp = fopen(query,"r");
  if( qp == NULL )
    error_ed(2);

  m = qlength;

  /// Array for keeping the query data
  Q = (double *)malloc(sizeof(double)*m);
  if( Q == NULL )
    error_ed(1);

  /// Read the query data from input file and calculate its statistic such as mean, std
  while(fscanf(qp,"%lf",&d) != EOF && i < m)
  {
    ex += d;
    ex2 += d*d;
    Q[i] = d;
    i++;
  }
  mean = ex/m;
  std = ex2/m;
  std = sqrt(std-mean*mean);
  fclose(qp);

  /// Do z_normalixation on query data
  for( i = 0 ; i < m ; i++ )
    Q[i] = (Q[i] - mean)/std;

  /// Sort the query data
  order = (int *)malloc(sizeof(int)*m);
  if( order == NULL )
    error_ed(1);
  Index *Q_tmp = (Index *)malloc(sizeof(Index)*m);
  if( Q_tmp == NULL )
    error_ed(1);
  for( i = 0 ; i < m ; i++ )
  {
    Q_tmp[i].value = Q[i];
    Q_tmp[i].index = i;
  }
  qsort(Q_tmp, m, sizeof(Index),comp_ed);
  for( i=0; i<m; i++)
  {   Q[i] = Q_tmp[i].value;
    order[i] = Q_tmp[i].index;
  }
  free(Q_tmp);



  /// Array for keeping the current data; Twice the size for removing modulo (circulation) in distance calculation
  T = (double *)malloc(sizeof(double)*2*m);
  if( T == NULL )
    error_ed(1);

  double dist = 0;
  i = 0;
  j = 0;
  ex = ex2 = 0;

  /// Read data file, one value at a time
  while(fscanf(fp,"%lf",&d) != EOF )
  {
    ex += d;
    ex2 += d*d;
    T[i%m] = d;
    T[(i%m)+m] = d;

    /// If there is enough data in T, the ED distance can be calculated
    if( i >= m-1 )
    {
      /// the current starting location of T
      j = (i+1)%m;

      /// Z_norm(T[i]) will be calculated on the fly
      mean = ex/m;
      std = ex2/m;
      std = sqrt(std-mean*mean);

      ///check for user input in case of run-away loop
      Rcpp::checkUserInterrupt();
      /// Calculate ED distance
      dist = distance(Q,T,j,m,mean,std,order,bsf);
      if( dist < bsf )
      {
        bsf = dist;
        loc = i-m+1;
      }
      ex -= T[j];
      ex2 -= T[j]*T[j];
    }
    i++;
  }
  fclose(fp);
  //t2 = clock();

  //Rcout << "Location : " << loc << endl;
  //Rcout << "Distance : " << sqrt(bsf) << endl;
  //Rcout << "Total Execution Time : " << (t2-t1)/CLOCKS_PER_SEC << " sec" << endl;
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("location") = loc + 1, // convert raw data location to R's 1-based indexing
                                      Rcpp::Named("distance") = sqrt(bsf));
  out.attr("class") = "ucred";
  return out;
}

//' UCR ED Algorithm file-vector method
//'
//' Sliding-window similarity search using Euclidean Distance. This implementation of the UCR Suite command line utility, takes a data file as input and an R numeric vector for the query.
//'
//' @name ucred_fv
//' @param data character; path to data file
//' @param query numeric vector containing query
//' @return a ucred object. A list with the following elements
//' \itemize{
//'   \item \strong{location:} The starting location of the nearest neighbor of the given query, of size \code{length(query)}, in the data. Note that location starts from 1.
//'   \item \strong{distance:} The Euclidean Distance between the nearest neighbor and the query.
//' }
//' @examples
//' #locate example data file
//' dataf <- system.file("extdata/col_sc.txt", package="rucrdtw")
//' #read example query file into vector
//' query <- scan(system.file("extdata/mid_sc.txt", package="rucrdtw"))
//' #run query
//' ucred_fv(dataf, query)
//' @useDynLib rucrdtw, .registration=TRUE
//' @importFrom Rcpp sourceCpp
//' @export
// [[Rcpp::export]]
Rcpp::List ucred_fv(const char * data , Rcpp::NumericVector query)
{
  FILE *fp;              // the input file pointer
  //FILE *qp;              // the query file pointer

  double *Q;             // query array
  double *T;             // array of current data
  int *order;            // ordering of query by |z(q_i)|
  double bsf;            // best-so-far
  int m;                 // length of query
  long long loc = 0;     // answer: location of the best-so-far match

  double d;
  long long i , j ;
  double ex , ex2 , mean, std;

  //double t1,t2;

  //t1 = clock();

  bsf = INF;
  i = 0;
  j = 0;
  ex = ex2 = 0;

  fp = fopen(data,"r");
  if( fp == NULL )
    error_ed(2);

  //qp = fopen(query,"r");
  //if( qp == NULL )
  //  error_ed(2);

  m = query.size();

  /// Array for keeping the query data
  Q = (double *)malloc(sizeof(double)*m);
  if( Q == NULL )
    error_ed(1);

  /// Read the query data from input file and calculate its statistic such as mean, std
  //while(fscanf(qp,"%lf",&d) != EOF && i < m)
    for (i = 0; i < m; i++)
    {
      ex += query[i];
      ex2 += query[i]*query[i];
      Q[i] = query[i];
    }
  mean = ex/m;
  std = ex2/m;
  std = sqrt(std-mean*mean);
  //fclose(qp);

  /// Do z_normalixation on query data
  for( i = 0 ; i < m ; i++ )
    Q[i] = (Q[i] - mean)/std;

  /// Sort the query data
  order = (int *)malloc(sizeof(int)*m);
  if( order == NULL )
    error_ed(1);
  Index *Q_tmp = (Index *)malloc(sizeof(Index)*m);
  if( Q_tmp == NULL )
    error_ed(1);
  for( i = 0 ; i < m ; i++ )
  {
    Q_tmp[i].value = Q[i];
    Q_tmp[i].index = i;
  }
  qsort(Q_tmp, m, sizeof(Index),comp_ed);
  for( i=0; i<m; i++)
  {   Q[i] = Q_tmp[i].value;
    order[i] = Q_tmp[i].index;
  }
  free(Q_tmp);



  /// Array for keeping the current data; Twice the size for removing modulo (circulation) in distance calculation
  T = (double *)malloc(sizeof(double)*2*m);
  if( T == NULL )
    error_ed(1);

  double dist = 0;
  i = 0;
  j = 0;
  ex = ex2 = 0;

  /// Read data file, one value at a time
  while(fscanf(fp,"%lf",&d) != EOF )
  {
    ex += d;
    ex2 += d*d;
    T[i%m] = d;
    T[(i%m)+m] = d;

    /// If there is enough data in T, the ED distance can be calculated
    if( i >= m-1 )
    {
      /// the current starting location of T
      j = (i+1)%m;

      /// Z_norm(T[i]) will be calculated on the fly
      mean = ex/m;
      std = ex2/m;
      std = sqrt(std-mean*mean);
      ///check for user input in case of run-away loop
      Rcpp::checkUserInterrupt();
      /// Calculate ED distance
      dist = distance(Q,T,j,m,mean,std,order,bsf);
      if( dist < bsf )
      {
        bsf = dist;
        loc = i-m+1;
      }
      ex -= T[j];
      ex2 -= T[j]*T[j];
    }
    i++;
  }
  fclose(fp);
  //t2 = clock();

  //Rcout << "Location : " << loc << endl;
  //Rcout << "Distance : " << sqrt(bsf) << endl;
  //Rcout << "Total Execution Time : " << (t2-t1)/CLOCKS_PER_SEC << " sec" << endl;
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("location") = loc + 1, // convert raw data location to R's 1-based indexing
                                      Rcpp::Named("distance") = sqrt(bsf));
  out.attr("class") = "ucred";
  return out;
}

//' UCR ED Algorithm vector-vector method
//'
//' Sliding-window similarity search using Euclidean Distance. This implementation of the UCR Suite Euclidean Distance command line utility takes an R numeric vector as data input and an R numeric vector for the query.
//'
//' @name ucred_vv
//' @param data numeric vector containing data
//' @param query numeric vector containing query
//' @param skip bool defaults to TRUE. If TRUE bound calculations and if necessary, distance calculations, are only performed on non-overlapping segments of the data (i.e. multiples of \code{length(query)}). This is useful if \code{data} is a set of multiple reference time series, each of length \code{length(query)}. The location returned when skipping is the index of the subsequence.
//' @return a ucred object. A list with the following elements
//' \itemize{
//'   \item \strong{location:} The starting location of the nearest neighbor of the given query, of size \code{length(query)}, in the data. Note that location starts from 1.
//'   \item \strong{distance:} The Euclidean Distance between the nearest neighbor and the query.
//' }
//' @examples
//' #read example file into vector
//' dataf <- scan(system.file("extdata/col_sc.txt", package="rucrdtw"))
//' #read example query file into vector
//' query <- scan(system.file("extdata/mid_sc.txt", package="rucrdtw"))
//' #run query
//' ucred_vv(dataf, query)
//' @useDynLib rucrdtw, .registration=TRUE
//' @importFrom Rcpp sourceCpp
//' @export
// [[Rcpp::export]]
Rcpp::List ucred_vv(Rcpp::NumericVector data , Rcpp::NumericVector query, bool skip = false)
{
  //FILE *fp;              // the input file pointer
  //FILE *qp;              // the query file pointer

  double *Q;             // query array
  double *T;             // array of current data
  int *order;            // ordering of query by |z(q_i)|
  double bsf;            // best-so-far
  int m;                 // length of query
  long long loc = 0;     // answer: location of the best-so-far match

  //double d;
  long long i , j , k;
  double ex , ex2 , mean, std;

  //double t1,t2;

  //t1 = clock();

  bsf = INF;
  i = 0;
  j = 0;
  ex = ex2 = 0;

 // fp = fopen(data,"r");
 // if( fp == NULL )
 //   error_ed(2);

  //qp = fopen(query,"r");
  //if( qp == NULL )
  //  error_ed(2);

  m = query.size();

  /// Array for keeping the query data
  Q = (double *)malloc(sizeof(double)*m);
  if( Q == NULL )
    error_ed(1);

  /// Read the query data from input file and calculate its statistic such as mean, std
  //while(fscanf(qp,"%lf",&d) != EOF && i < m)
  for (i = 0; i < m; i++)
  {
    ex += query[i];
    ex2 += query[i]*query[i];
    Q[i] = query[i];
  }
  mean = ex/m;
  std = ex2/m;
  std = sqrt(std-mean*mean);
  //fclose(qp);

  /// Do z_normalixation on query data
  for( i = 0 ; i < m ; i++ )
    Q[i] = (Q[i] - mean)/std;

  /// Sort the query data
  order = (int *)malloc(sizeof(int)*m);
  if( order == NULL )
    error_ed(1);
  Index *Q_tmp = (Index *)malloc(sizeof(Index)*m);
  if( Q_tmp == NULL )
    error_ed(1);
  for( i = 0 ; i < m ; i++ )
  {
    Q_tmp[i].value = Q[i];
    Q_tmp[i].index = i;
  }
  qsort(Q_tmp, m, sizeof(Index),comp_ed);
  for( i=0; i<m; i++)
  {   Q[i] = Q_tmp[i].value;
    order[i] = Q_tmp[i].index;
  }
  free(Q_tmp);



  /// Array for keeping the current data; Twice the size for removing modulo (circulation) in distance calculation
  T = (double *)malloc(sizeof(double)*2*m);
  if( T == NULL )
    error_ed(1);

  double dist = 0;
  i = 0;
  j = 0;
  k = 0;
  ex = ex2 = 0;

  /// Read data file, one value at a time
  //while(fscanf(fp,"%lf",&d) != EOF )
  for(k=0; k < data.size(); k++)
  {
    ex += data[k];
    ex2 += data[k]*data[k];
    T[i%m] = data[k];
    T[(i%m)+m] = data[k];

    /// If there is enough data in T, the ED distance can be calculated
    if( i >= m-1 )
    {
      /// the current starting location of T
      j = (i+1)%m;

      /// Z_norm(T[i]) will be calculated on the fly
      mean = ex/m;
      std = ex2/m;
      std = sqrt(std-mean*mean);

      if (!skip || (j==0)){
        //if (skip) Rcout << k << "; i " << i << "; " << j << "; ";// for debugging
        ///check for user input in case of run-away loop
        Rcpp::checkUserInterrupt();
      /// Calculate ED distance
      dist = distance(Q,T,j,m,mean,std,order,bsf);
      //if (skip) Rcout << dist << endl;//for debugging
      if( dist < bsf )
      {
        bsf = dist;
        loc = i-m+1;
      }
      }
      ex -= T[j];
      ex2 -= T[j]*T[j];
    }
    i++;
  }
//  fclose(fp);
  //t2 = clock();

  //Rcout << "Location : " << loc << endl;
  //Rcout << "Distance : " << sqrt(bsf) << endl;
  //Rcout << "Total Execution Time : " << (t2-t1)/CLOCKS_PER_SEC << " sec" << endl;
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("location") = (skip) ? (loc / m + 1) : (loc + 1), //loc is index of subsequence if skipping, otherwise convert raw data location to R's 1-based indexing
                                      Rcpp::Named("distance") = sqrt(bsf));
  out.attr("class") = "ucred";
  return out;
}
