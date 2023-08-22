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

#include <Rcpp.h>



#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>

#define min(x,y) ((x)<(y)?(x):(y))
#define max(x,y) ((x)>(y)?(x):(y))
#define dist(x,y) ((x-y)*(x-y))


#define INF 1e20       //Pseudo Infitinte number for this code

//dealing with the CRAN long long issue
#if defined(__GNUC__) &&  defined(__LONG_LONG_MAX__)
__extension__ typedef long long int rcpp_long_long_type;
__extension__ typedef unsigned long long int rcpp_ulong_long_type;
#define RCPP_HAS_LONG_LONG_TYPES
#endif

using namespace std;
using namespace Rcpp;

/// Data structure for sorting the query
typedef struct Index
    {   double value;
        int    index;
    } Index;

/// Data structure (circular array) for finding minimum and maximum for LB_Keogh envolop
struct deque2
{   int *dq;
    int size,capacity;
    int f,r;
};


/// Sorting function for the query, sort by abs(z_norm(q[i])) from high to low
int comp(const void *a, const void* b)
{   Index* x = (Index*)a;
    Index* y = (Index*)b;
    return fabs(y->value) - fabs(x->value);   // high to low
}

/// Initial the queue at the begining step of envelop calculation
void init(deque2 *d, int capacity)
{
    d->capacity = capacity;
    d->size = 0;
    d->dq = (int *) malloc(sizeof(int)*d->capacity);
    d->f = 0;
    d->r = d->capacity-1;
}

/// Destroy the queue
void destroy(deque2 *d)
{
    free(d->dq);
}

/// Insert to the queue at the back
void push_back(struct deque2 *d, int v)
{
    d->dq[d->r] = v;
    d->r--;
    if (d->r < 0)
        d->r = d->capacity-1;
    d->size++;
}

/// Delete the current (front) element from queue
void pop_front(struct deque2 *d)
{
    d->f--;
    if (d->f < 0)
        d->f = d->capacity-1;
    d->size--;
}

/// Delete the last element from queue
void pop_back(struct deque2 *d)
{
    d->r = (d->r+1)%d->capacity;
    d->size--;
}

/// Get the value at the current position of the circular queue
int front(struct deque2 *d)
{
    int aux = d->f - 1;

    if (aux < 0)
        aux = d->capacity-1;
    return d->dq[aux];
}

/// Get the value at the last position of the circular queueint back(struct deque *d)
int back(struct deque2 *d)
{
    int aux = (d->r+1)%d->capacity;
    return d->dq[aux];
}

/// Check whether or not the queue is empty
int empty(struct deque2 *d)
{
    return d->size == 0;
}

/// Finding the envelop of min and max value for LB_Keogh
/// Implementation idea is intoruduced by Danial Lemire in his paper
/// "Faster Retrieval with a Two-Pass Dynamic-Time-Warping Lower Bound", Pattern Recognition 42(9), 2009.
void lower_upper_lemire(double *t, int len, int r, double *l, double *u)
{
    struct deque2 du;
    struct deque2 dl;

    init(&du, 2*r+2);
    init(&dl, 2*r+2);

    push_back(&du, 0);
    push_back(&dl, 0);

    for (int i = 1; i < len; i++)
    {
        if (i > r)
        {
            u[i-r-1] = t[front(&du)];
            l[i-r-1] = t[front(&dl)];
        }
        if (t[i] > t[i-1])
        {
            pop_back(&du);
            while (!empty(&du) && t[i] > t[back(&du)])
                pop_back(&du);
        }
        else
        {
            pop_back(&dl);
            while (!empty(&dl) && t[i] < t[back(&dl)])
                pop_back(&dl);
        }
        push_back(&du, i);
        push_back(&dl, i);
        if (i == 2 * r + 1 + front(&du))
            pop_front(&du);
        else if (i == 2 * r + 1 + front(&dl))
            pop_front(&dl);
    }
    for (int i = len; i < len+r+1; i++)
    {
        u[i-r-1] = t[front(&du)];
        l[i-r-1] = t[front(&dl)];
        if (i-front(&du) >= 2 * r + 1)
            pop_front(&du);
        if (i-front(&dl) >= 2 * r + 1)
            pop_front(&dl);
    }
    destroy(&du);
    destroy(&dl);
}

/// Calculate quick lower bound
/// Usually, LB_Kim take time O(m) for finding top,bottom,fist and last.
/// However, because of z-normalization the top and bottom cannot give siginifant benefits.
/// And using the first and last points can be computed in constant time.
/// The prunning power of LB_Kim is non-trivial, especially when the query is not long, say in length 128.
double lb_kim_hierarchy(double *t, double *q, int j, int len, double mean, double std, double bsf = INF)
{
    /// 1 point at front and back
    double d, lb;
    double x0 = (t[j] - mean) / std;
    double y0 = (t[(len-1+j)] - mean) / std;
    lb = dist(x0,q[0]) + dist(y0,q[len-1]);
    if (lb >= bsf)   return lb;

    /// 2 points at front
    double x1 = (t[(j+1)] - mean) / std;
    d = min(dist(x1,q[0]), dist(x0,q[1]));
    d = min(d, dist(x1,q[1]));
    lb += d;
    if (lb >= bsf)   return lb;

    /// 2 points at back
    double y1 = (t[(len-2+j)] - mean) / std;
    d = min(dist(y1,q[len-1]), dist(y0, q[len-2]) );
    d = min(d, dist(y1,q[len-2]));
    lb += d;
    if (lb >= bsf)   return lb;

    /// 3 points at front
    double x2 = (t[(j+2)] - mean) / std;
    d = min(dist(x0,q[2]), dist(x1, q[2]));
    d = min(d, dist(x2,q[2]));
    d = min(d, dist(x2,q[1]));
    d = min(d, dist(x2,q[0]));
    lb += d;
    if (lb >= bsf)   return lb;

    /// 3 points at back
    double y2 = (t[(len-3+j)] - mean) / std;
    d = min(dist(y0,q[len-3]), dist(y1, q[len-3]));
    d = min(d, dist(y2,q[len-3]));
    d = min(d, dist(y2,q[len-2]));
    d = min(d, dist(y2,q[len-1]));
    lb += d;

    return lb;
}

/// LB_Keogh 1: Create Envelop for the query
/// Note that because the query is known, envelop can be created once at the begenining.
///
/// Variable Explanation,
/// order : sorted indices for the query.
/// uo, lo: upper and lower envelops for the query, which already sorted.
/// t     : a circular array keeping the current data.
/// j     : index of the starting location in t
/// cb    : (output) current bound at each position. It will be used later for early abandoning in DTW.
double lb_keogh_cumulative(int* order, double *t, double *uo, double *lo, double *cb, int j, int len, double mean, double std, double best_so_far = INF)
{
    double lb = 0;
    double x, d;

    for (int i = 0; i < len && lb < best_so_far; i++)
    {
        x = (t[(order[i]+j)] - mean) / std;
        d = 0;
        if (x > uo[i])
            d = dist(x,uo[i]);
        else if(x < lo[i])
            d = dist(x,lo[i]);
        lb += d;
        cb[order[i]] = d;
    }
    return lb;
}

/// LB_Keogh 2: Create Envelop for the data
/// Note that the envelops have been created (in main function) when each data point has been read.
///
/// Variable Explanation,
/// tz: Z-normalized data
/// qo: sorted query
/// cb: (output) current bound at each position. Used later for early abandoning in DTW.
/// l,u: lower and upper envelop of the current data
double lb_keogh_data_cumulative(int* order, double *tz, double *qo, double *cb, double *l, double *u, int len, double mean, double std, double best_so_far = INF)
{
    double lb = 0;
    double uu,ll,d;

    for (int i = 0; i < len && lb < best_so_far; i++)
    {
        uu = (u[order[i]]-mean)/std;
        ll = (l[order[i]]-mean)/std;
        d = 0;
        if (qo[i] > uu)
            d = dist(qo[i], uu);
        else
        {   if(qo[i] < ll)
            d = dist(qo[i], ll);
        }
        lb += d;
        cb[order[i]] = d;
    }
    return lb;
}

/// Calculate Dynamic Time Wrapping distance
/// A,B: data and query, respectively
/// cb : cummulative bound used for early abandoning
/// r  : size of Sakoe-Chiba warpping band
double dtw(double* A, double* B, double *cb, int m, int r, double bsf = INF)
{

    double *cost;
    double *cost_prev;
    double *cost_tmp;
    int i,j,k;
    double x,y,z,min_cost;

    /// Instead of using matrix of size O(m^2) or O(mr), we will reuse two array of size O(r).
    cost = (double*)malloc(sizeof(double)*(2*r+1));
    for(k=0; k<2*r+1; k++)    cost[k]=INF;

    cost_prev = (double*)malloc(sizeof(double)*(2*r+1));
    for(k=0; k<2*r+1; k++)    cost_prev[k]=INF;

    for (i=0; i<m; i++)
    {
        k = max(0,r-i);
        min_cost = INF;

        for(j=max(0,i-r); j<=min(m-1,i+r); j++, k++)
        {
            /// Initialize all row and column
            if ((i==0)&&(j==0))
            {
                cost[k]=dist(A[0],B[0]);
                min_cost = cost[k];
                continue;
            }

            if ((j-1<0)||(k-1<0))     y = INF;
            else                      y = cost[k-1];
            if ((i-1<0)||(k+1>2*r))   x = INF;
            else                      x = cost_prev[k+1];
            if ((i-1<0)||(j-1<0))     z = INF;
            else                      z = cost_prev[k];

            /// Classic DTW calculation
            cost[k] = min( min( x, y) , z) + dist(A[i],B[j]);

            /// Find minimum cost in row for early abandoning (possibly to use column instead of row).
            if (cost[k] < min_cost)
            {   min_cost = cost[k];
            }
        }

        /// We can abandon early if the current cummulative distace with lower bound together are larger than bsf
        if (i+r < m-1 && min_cost + cb[i+r+1] >= bsf)
        {   free(cost);
            free(cost_prev);
            return min_cost + cb[i+r+1];
        }

        /// Move current array to previous array.
        cost_tmp = cost;
        cost = cost_prev;
        cost_prev = cost_tmp;
    }
    k--;

    /// the DTW distance is in the last cell in the matrix of size O(m^2) or at the middle of our array.
    double final_dtw = cost_prev[k];
    free(cost);
    free(cost_prev);
    return final_dtw;
}

/// Print function for debugging
//void printArray(double *x, int len)
//{   for(int i=0; i<len; i++)
//        Rprintf(" %6.2lf",x[i]);
//    Rprintf("\n");
//}

/// If expected error happens, teminated the program.
void error(int id)
{
    if(id==1)
        stop("ERROR : Memory can't be allocated!!!\n\n");
    else if ( id == 2 )
        stop("ERROR : File not Found!!!\n\n");
}



/// Just the main function adapted for Rcpp export
//' UCR DTW Algorithm file-file method
//'
//' Sliding-window similarity search using DTW distance. This implementation is very close to the UCR Suite command line utility, in that it takes files as inputs for both query and data
//'
//' @name ucrdtw_ff
//' @param data character; path to data file
//' @param query character; path to query file
//' @param qlength integer; length of the query (number of data points), usually this should be equivalent to length(query), but it can be shorter.
//' @param dtwwindow double; Size of the warping window size (as a proportion of query length). The DTW calculation in `rucrdtw` uses a symmetric Sakoe-Chiba band. See Giorgino (2009) for a general coverage of warping window constraints.
//' @return a ucrdtw object. A list with the following elements
//' \itemize{
//'   \item \strong{location:} The starting location of the nearest neighbor of the given query, of size \code{length(query)}, in the data. Note that location starts from 1.
//'   \item \strong{distance:} The DTW distance between the nearest neighbor and the query.
//'   \item \strong{prunedKim:} Percentage of subsequences that were pruned based on the LB-Kim criterion.
//'   \item \strong{prunedKeogh:} Percentage of subsequences that were pruned based on the LB-Keogh-EQ criterion.
//'   \item \strong{prunedKeogh2:} Percentage of subsequences that were pruned based on the LB-Keogh-EC criterion.
//'   \item \strong{dtwCalc:} Percentage of subsequences for which the full DTW distance was calculated.
//' }
//' For an explanation of the pruning criteria see Rakthanmanon et al. (2012).
//' @references Rakthanmanon, Thanawin, Bilson Campana, Abdullah Mueen, Gustavo Batista, Brandon Westover, Qiang Zhu, Jesin Zakaria, and Eamonn Keogh. 2012. Searching and Mining Trillions of Time Series Subsequences Under Dynamic Time Warping. In Proceedings of the 18th ACM SIGKDD International Conference on Knowledge Discovery and Data Mining, 262-70. ACM. doi:\doi{10.1145/2339530.2339576}.
//' @references Giorgino, Toni (2009). Computing and Visualizing Dynamic Time Warping Alignments in R: The dtw Package. Journal of Statistical Software, 31(7), 1-24, doi:\doi{10.18637/jss.v031.i07}.
//' @examples
//' #locate example data file
//' dataf <- system.file("extdata/col_sc.txt", package="rucrdtw")
//' #locate example query file
//' queryf <- system.file("extdata/mid_sc.txt", package="rucrdtw")
//' #determine length of query file
//' qlength <- length(scan(queryf))
//' #run query
//' ucrdtw_ff(dataf, queryf, qlength, 0.05)
//' @useDynLib rucrdtw, .registration=TRUE
//' @importFrom Rcpp sourceCpp
//' @export
// [[Rcpp::export]]
Rcpp::List ucrdtw_ff(const char * data , const char * query, int qlength, double dtwwindow)
{
  FILE *fp;            /// data file pointer
  FILE *qp;            /// query file pointer
  double bsf;          /// best-so-far
  double *t, *q;       /// data array and query array
  int *order;          ///new order of the query
  double *u, *l, *qo, *uo, *lo,*tz,*cb, *cb1, *cb2,*u_d, *l_d;


  double d;
  rcpp_long_long_type i , j;
  double ex , ex2 , mean, std;
  int m=-1, r=-1;
  rcpp_long_long_type loc = 0;
  //double t1,t2;
  int kim = 0,keogh = 0, keogh2 = 0;
  double dist=0, lb_kim=0, lb_k=0, lb_k2=0;
  double *buffer, *u_buff, *l_buff;
  Index *Q_tmp;

  /// For every EPOCH points, all cummulative values, such as ex (sum), ex2 (sum square), will be restarted for reducing the floating point error.
  int EPOCH = 100000;

  /// If not enough input, display an error.
  //if (argc<=3)
  //  error(4);

  /// read size of the query
  m = qlength;

  /// read warping windows
  {   double R = dtwwindow;
    if (R<=1)
      r = floor(R*m);
    else
      r = floor(R);
  }

  fp = fopen(data,"r");
  if( fp == NULL )
    error(2);

  qp = fopen(query,"r");
  if( qp == NULL )
    error(2);

  /// start the clock
  //t1 = clock();


  /// malloc everything here
  q = (double *)malloc(sizeof(double)*m);
  if( q == NULL )
    error(1);
  qo = (double *)malloc(sizeof(double)*m);
  if( qo == NULL )
    error(1);
  uo = (double *)malloc(sizeof(double)*m);
  if( uo == NULL )
    error(1);
  lo = (double *)malloc(sizeof(double)*m);
  if( lo == NULL )
    error(1);

  order = (int *)malloc(sizeof(int)*m);
  if( order == NULL )
    error(1);

  Q_tmp = (Index *)malloc(sizeof(Index)*m);
  if( Q_tmp == NULL )
    error(1);

  u = (double *)malloc(sizeof(double)*m);
  if( u == NULL )
    error(1);

  l = (double *)malloc(sizeof(double)*m);
  if( l == NULL )
    error(1);

  cb = (double *)malloc(sizeof(double)*m);
  if( cb == NULL )
    error(1);

  cb1 = (double *)malloc(sizeof(double)*m);
  if( cb1 == NULL )
    error(1);

  cb2 = (double *)malloc(sizeof(double)*m);
  if( cb2 == NULL )
    error(1);

  u_d = (double *)malloc(sizeof(double)*m);
  if( u == NULL )
    error(1);

  l_d = (double *)malloc(sizeof(double)*m);
  if( l == NULL )
    error(1);

  t = (double *)malloc(sizeof(double)*m*2);
  if( t == NULL )
    error(1);

  tz = (double *)malloc(sizeof(double)*m);
  if( tz == NULL )
    error(1);

  buffer = (double *)malloc(sizeof(double)*EPOCH);
  if( buffer == NULL )
    error(1);

  u_buff = (double *)malloc(sizeof(double)*EPOCH);
  if( u_buff == NULL )
    error(1);

  l_buff = (double *)malloc(sizeof(double)*EPOCH);
  if( l_buff == NULL )
    error(1);


  /// Read query file
  bsf = INF;
  i = 0;
  j = 0;
  ex = ex2 = 0;

  while(fscanf(qp,"%lf",&d) != EOF && i < m)
  {
    ex += d;
    ex2 += d*d;
    q[i] = d;
    i++;
  }
  fclose(qp);

  /// Do z-normalize the query, keep in same array, q
  mean = ex/m;
  std = ex2/m;
  std = sqrt(std-mean*mean);
  for( i = 0 ; i < m ; i++ )
    q[i] = (q[i] - mean)/std;

  /// Create envelop of the query: lower envelop, l, and upper envelop, u
  lower_upper_lemire(q, m, r, l, u);

  /// Sort the query one time by abs(z-norm(q[i]))
  for( i = 0; i<m; i++)
  {
    Q_tmp[i].value = q[i];
    Q_tmp[i].index = i;
  }
  qsort(Q_tmp, m, sizeof(Index),comp);

  /// also create another arrays for keeping sorted envelop
  for( i=0; i<m; i++)
  {   int o = Q_tmp[i].index;
    order[i] = o;
    qo[i] = q[o];
    uo[i] = u[o];
    lo[i] = l[o];
  }
  free(Q_tmp);

  /// Initial the cummulative lower bound
  for( i=0; i<m; i++)
  {   cb[i]=0;
    cb1[i]=0;
    cb2[i]=0;
  }

  i = 0;          /// current index of the data in current chunk of size EPOCH
  j = 0;          /// the starting index of the data in the circular array, t
  ex = ex2 = 0;
  bool done = false;
  int it=0, ep=0, k=0;
  rcpp_long_long_type I;    /// the starting index of the data in current chunk of size EPOCH

  while(!done)
  {
    /// Read first m-1 points
    ep=0;
    if (it==0)
    {   for(k=0; k<m-1; k++)
      if (fscanf(fp,"%lf",&d) != EOF)
        buffer[k] = d;
    }
    else
    {   for(k=0; k<m-1; k++)
      buffer[k] = buffer[EPOCH-m+1+k];
    }

    /// Read buffer of size EPOCH or when all data has been read.
    ep=m-1;
    while(ep<EPOCH)
    {   if (fscanf(fp,"%lf",&d) == EOF)
      break;
    buffer[ep] = d;
    ep++;
    }

    /// Data are read in chunk of size EPOCH.
    /// When there is nothing to read, the loop is end.
    if (ep<=m-1)
    {   done = true;
    } else
    {   lower_upper_lemire(buffer, ep, r, l_buff, u_buff);

      /// Just for printing a dot for approximate a million point. Not much accurate.
      //if (it%(1000000/(EPOCH-m+1))==0)
        //REprintf(".");

      /// Do main task here..
      ex=0; ex2=0;
      for(i=0; i<ep; i++)
      {
        /// A bunch of data has been read and pick one of them at a time to use
        d = buffer[i];

        /// Calcualte sum and sum square
        ex += d;
        ex2 += d*d;

        /// t is a circular array for keeping current data
        t[i%m] = d;

        /// Double the size for avoiding using modulo "%" operator
        t[(i%m)+m] = d;

        /// Start the task when there are more than m-1 points in the current chunk
        if( i >= m-1 )
        {
          mean = ex/m;
          std = ex2/m;
          std = sqrt(std-mean*mean);

          /// compute the start location of the data in the current circular array, t
          j = (i+1)%m;
          /// the start location of the data in the current chunk
          I = i-(m-1);

          /// Use a constant lower bound to prune the obvious subsequence
          lb_kim = lb_kim_hierarchy(t, q, j, m, mean, std, bsf);

          if (lb_kim < bsf)
          {
            /// Use a linear time lower bound to prune; z_normalization of t will be computed on the fly.
            /// uo, lo are envelop of the query.
            lb_k = lb_keogh_cumulative(order, t, uo, lo, cb1, j, m, mean, std, bsf);
            if (lb_k < bsf)
            {
              /// Take another linear time to compute z_normalization of t.
              /// Note that for better optimization, this can merge to the previous function.
              for(k=0;k<m;k++)
              {   tz[k] = (t[(k+j)] - mean)/std;
              }

              /// Use another lb_keogh to prune
              /// qo is the sorted query. tz is unsorted z_normalized data.
              /// l_buff, u_buff are big envelop for all data in this chunk
              lb_k2 = lb_keogh_data_cumulative(order, tz, qo, cb2, l_buff+I, u_buff+I, m, mean, std, bsf);
              if (lb_k2 < bsf)
              {
                /// Choose better lower bound between lb_keogh and lb_keogh2 to be used in early abandoning DTW
                /// Note that cb and cb2 will be cumulative summed here.
                if (lb_k > lb_k2)
                {
                  cb[m-1]=cb1[m-1];
                  for(k=m-2; k>=0; k--)
                    cb[k] = cb[k+1]+cb1[k];
                }
                else
                {
                  cb[m-1]=cb2[m-1];
                  for(k=m-2; k>=0; k--)
                    cb[k] = cb[k+1]+cb2[k];
                }

                /// Compute DTW and early abandoning if possible
                dist = dtw(tz, q, cb, m, r, bsf);

                if( dist < bsf )
                {   /// Update bsf
                  /// loc is the real starting location of the nearest neighbor in the file
                  bsf = dist;
                  loc = (it)*(EPOCH-m+1) + i-m+1;
                }
              } else
                keogh2++;
            } else
              keogh++;
          } else
            kim++;

          /// Reduce obsolute points from sum and sum square
          ex -= t[j];
          ex2 -= t[j]*t[j];
        }
      }

      /// If the size of last chunk is less then EPOCH, then no more data and terminate.
      if (ep<EPOCH)
        done=true;
      else
        it++;
    }
  }

  i = (it)*(EPOCH-m+1) + ep;
  fclose(fp);

  free(q);
  free(u);
  free(l);
  free(uo);
  free(lo);
  free(qo);
  free(cb);
  free(cb1);
  free(cb2);
  free(tz);
  free(t);
  free(l_d);
  free(u_d);
  free(l_buff);
  free(u_buff);
  free(order);
  free(buffer);

  //t2 = clock();
  //Rprintf("\n");

  /// Note that loc and i are rcpp_long_long_type.
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("location") = loc + 1, //convert to R's 1-based indexing
                                      Rcpp::Named("distance") = sqrt(bsf),
                                      //Rcpp::Named("exec_time")=(t2-t1)/CLOCKS_PER_SEC,
                                      Rcpp::Named("prunedKim")=((double) kim / i)*100,
                                      Rcpp::Named("prunedKeogh")=((double) keogh / i)*100,
                                      Rcpp::Named("prunedKeogh2")=((double) keogh2 / i)*100,
                                      Rcpp::Named("dtwCalc")=100-(((double)kim+keogh+keogh2)/i*100));
  out.attr("class") = "ucrdtw";
  return out;
}

/// Just the main function adapted for Rcpp export
//' UCR DTW Algorithm file-vector method
//'
//' Sliding-window similarity search using DTW distance. This implementation of the UCR Suite command line utility, takes a data file as input and an R numeric vector for the query
//'
//' @name ucrdtw_fv
//' @param data character; path to data file
//' @param query numeric vector containing the query. The query length is set to the length of this object.
//' @param dtwwindow double; Size of the warping window size (as a proportion of query length). The DTW calculation in `rucrdtw` uses a symmetric Sakoe-Chiba band. See Giorgino (2009) for a general coverage of warping window constraints.
//' @return a ucrdtw object. A list with the following elements
//' \itemize{
//'   \item \strong{location:} The starting location of the nearest neighbor of the given query, of size \code{length(query)}, in the data. Note that location starts from 1.
//'   \item \strong{distance:} The DTW distance between the nearest neighbor and the query.
//'   \item \strong{prunedKim:} Percentage of subsequences that were pruned based on the LB-Kim criterion.
//'   \item \strong{prunedKeogh:} Percentage of subsequences that were pruned based on the LB-Keogh-EQ criterion.
//'   \item \strong{prunedKeogh2:} Percentage of subsequences that were pruned based on the LB-Keogh-EC criterion.
//'   \item \strong{dtwCalc:} Percentage of subsequences for which the full DTW distance was calculated.
//' }
//' For an explanation of the pruning criteria see Rakthanmanon et al. (2012).
//' @references Rakthanmanon, Thanawin, Bilson Campana, Abdullah Mueen, Gustavo Batista, Brandon Westover, Qiang Zhu, Jesin Zakaria, and Eamonn Keogh. 2012. Searching and Mining Trillions of Time Series Subsequences Under Dynamic Time Warping. In Proceedings of the 18th ACM SIGKDD International Conference on Knowledge Discovery and Data Mining, 262-70. ACM. doi:\doi{10.1145/2339530.2339576}.
//' @references Giorgino, Toni (2009). Computing and Visualizing Dynamic Time Warping Alignments in R: The dtw Package. Journal of Statistical Software, 31(7), 1-24, doi:\doi{10.18637/jss.v031.i07}.
//' @examples
//' #locate example data file
//' dataf <- system.file("extdata/col_sc.txt", package="rucrdtw")
//' #load example data set
//' data("synthetic_control")
//' #extract first row as query
//' query <- synthetic_control[1,]
//' #run query
//' ucrdtw_fv(dataf, query, 0.05)
//' @useDynLib rucrdtw, .registration=TRUE
//' @importFrom Rcpp sourceCpp
//' @export
// [[Rcpp::export]]
Rcpp::List ucrdtw_fv(const char * data , Rcpp::NumericVector query, double dtwwindow)
{
  FILE *fp;            /// data file pointer
  //FILE *qp;            /// query file pointer
  double bsf;          /// best-so-far
  double *t, *q;       /// data array and query array
  int *order;          ///new order of the query
  double *u, *l, *qo, *uo, *lo,*tz,*cb, *cb1, *cb2,*u_d, *l_d;


  double d;
  rcpp_long_long_type i , j;
  double ex , ex2 , mean, std;
  int m=-1, r=-1;
  rcpp_long_long_type loc = 0;
  //double t1,t2;
  int kim = 0,keogh = 0, keogh2 = 0;
  double dist=0, lb_kim=0, lb_k=0, lb_k2=0;
  double *buffer, *u_buff, *l_buff;
  Index *Q_tmp;

  /// For every EPOCH points, all cummulative values, such as ex (sum), ex2 (sum square), will be restarted for reducing the floating point error.
  int EPOCH = 100000;

  /// If not enough input, display an error.
  //if (argc<=3)
  //  error(4);

  /// read size of the query
  m = query.size();

  /// read warping windows
  {   double R = dtwwindow;
    if (R<=1)
      r = floor(R*m);
    else
      r = floor(R);
  }

  fp = fopen(data,"r");
  if( fp == NULL )
    error(2);

  //Rcpp::Rcout << "opened data file \n";
  //qp = fopen(query,"r");
  //if( qp == NULL )
  //error(2);

  /// start the clock
  //t1 = clock();


  /// malloc everything here
  q = (double *)malloc(sizeof(double)*m);
  if( q == NULL )
    error(1);
  qo = (double *)malloc(sizeof(double)*m);
  if( qo == NULL )
    error(1);
  uo = (double *)malloc(sizeof(double)*m);
  if( uo == NULL )
    error(1);
  lo = (double *)malloc(sizeof(double)*m);
  if( lo == NULL )
    error(1);

  order = (int *)malloc(sizeof(int)*m);
  if( order == NULL )
    error(1);

  Q_tmp = (Index *)malloc(sizeof(Index)*m);
  if( Q_tmp == NULL )
    error(1);

  u = (double *)malloc(sizeof(double)*m);
  if( u == NULL )
    error(1);

  l = (double *)malloc(sizeof(double)*m);
  if( l == NULL )
    error(1);

  cb = (double *)malloc(sizeof(double)*m);
  if( cb == NULL )
    error(1);

  cb1 = (double *)malloc(sizeof(double)*m);
  if( cb1 == NULL )
    error(1);

  cb2 = (double *)malloc(sizeof(double)*m);
  if( cb2 == NULL )
    error(1);

  u_d = (double *)malloc(sizeof(double)*m);
  if( u == NULL )
    error(1);

  l_d = (double *)malloc(sizeof(double)*m);
  if( l == NULL )
    error(1);

  t = (double *)malloc(sizeof(double)*m*2);
  if( t == NULL )
    error(1);

  tz = (double *)malloc(sizeof(double)*m);
  if( tz == NULL )
    error(1);

  buffer = (double *)malloc(sizeof(double)*EPOCH);
  if( buffer == NULL )
    error(1);

  u_buff = (double *)malloc(sizeof(double)*EPOCH);
  if( u_buff == NULL )
    error(1);

  l_buff = (double *)malloc(sizeof(double)*EPOCH);
  if( l_buff == NULL )
    error(1);


  /// Read query file
  bsf = INF;
  i = 0;
  j = 0;
  ex = ex2 = 0;

  //while(fscanf(qp,"%lf",&d) != EOF && i < m)
  //Rcpp::Rcout << "attempting to fill query\n";
  for (i = 0; i < m; i++)
  {
    ex += query[i];
    ex2 += query[i]*query[i];
    q[i] = query[i];
  }
  //Rcpp::Rcout << *q << "\n";
  //fclose(qp);

  /// Do z-normalize the query, keep in same array, q
  mean = ex/m;
  std = ex2/m;
  std = sqrt(std-mean*mean);
  for( i = 0 ; i < m ; i++ )
    q[i] = (q[i] - mean)/std;
  //Rcpp::Rcout << "done with normalisation\n";
  /// Create envelop of the query: lower envelop, l, and upper envelop, u
  lower_upper_lemire(q, m, r, l, u);

  /// Sort the query one time by abs(z-norm(q[i]))
  for( i = 0; i<m; i++)
  {
    Q_tmp[i].value = q[i];
    Q_tmp[i].index = i;
  }
  qsort(Q_tmp, m, sizeof(Index),comp);

  /// also create another arrays for keeping sorted envelop
  for( i=0; i<m; i++)
  {   int o = Q_tmp[i].index;
    order[i] = o;
    qo[i] = q[o];
    uo[i] = u[o];
    lo[i] = l[o];
  }
  free(Q_tmp);

  /// Initial the cummulative lower bound
  for( i=0; i<m; i++)
  {   cb[i]=0;
    cb1[i]=0;
    cb2[i]=0;
  }

  i = 0;          /// current index of the data in current chunk of size EPOCH
  j = 0;          /// the starting index of the data in the circular array, t
  ex = ex2 = 0;
  bool done = false;
  int it=0, ep=0, k=0;
  rcpp_long_long_type I;    /// the starting index of the data in current chunk of size EPOCH

  while(!done)
  {
    /// Read first m-1 points
    ep=0;
    if (it==0)
    {   for(k=0; k<m-1; k++)
      if (fscanf(fp,"%lf",&d) != EOF)
        buffer[k] = d;
    }
    else
    {   for(k=0; k<m-1; k++)
      buffer[k] = buffer[EPOCH-m+1+k];
    }

    /// Read buffer of size EPOCH or when all data has been read.
    ep=m-1;
    while(ep<EPOCH)
    {   if (fscanf(fp,"%lf",&d) == EOF)
      break;
    buffer[ep] = d;
    ep++;
    }

    /// Data are read in chunk of size EPOCH.
    /// When there is nothing to read, the loop is end.
    if (ep<=m-1)
    {   done = true;
    } else
    {   lower_upper_lemire(buffer, ep, r, l_buff, u_buff);

      /// Just for printing a dot for approximate a million point. Not much accurate.
      //if (it%(1000000/(EPOCH-m+1))==0)
        //REprintf(".");

      /// Do main task here..
      ex=0; ex2=0;
      for(i=0; i<ep; i++)
      {
        /// A bunch of data has been read and pick one of them at a time to use
        d = buffer[i];

        /// Calcualte sum and sum square
        ex += d;
        ex2 += d*d;

        /// t is a circular array for keeping current data
        t[i%m] = d;

        /// Double the size for avoiding using modulo "%" operator
        t[(i%m)+m] = d;

        /// Start the task when there are more than m-1 points in the current chunk
        if( i >= m-1 )
        {
          mean = ex/m;
          std = ex2/m;
          std = sqrt(std-mean*mean);

          /// compute the start location of the data in the current circular array, t
          j = (i+1)%m;
          /// the start location of the data in the current chunk
          I = i-(m-1);

          /// Use a constant lower bound to prune the obvious subsequence
          lb_kim = lb_kim_hierarchy(t, q, j, m, mean, std, bsf);

          if (lb_kim < bsf)
          {
            /// Use a linear time lower bound to prune; z_normalization of t will be computed on the fly.
            /// uo, lo are envelop of the query.
            lb_k = lb_keogh_cumulative(order, t, uo, lo, cb1, j, m, mean, std, bsf);
            if (lb_k < bsf)
            {
              /// Take another linear time to compute z_normalization of t.
              /// Note that for better optimization, this can merge to the previous function.
              for(k=0;k<m;k++)
              {   tz[k] = (t[(k+j)] - mean)/std;
              }

              /// Use another lb_keogh to prune
              /// qo is the sorted query. tz is unsorted z_normalized data.
              /// l_buff, u_buff are big envelop for all data in this chunk
              lb_k2 = lb_keogh_data_cumulative(order, tz, qo, cb2, l_buff+I, u_buff+I, m, mean, std, bsf);
              if (lb_k2 < bsf)
              {
                /// Choose better lower bound between lb_keogh and lb_keogh2 to be used in early abandoning DTW
                /// Note that cb and cb2 will be cumulative summed here.
                if (lb_k > lb_k2)
                {
                  cb[m-1]=cb1[m-1];
                  for(k=m-2; k>=0; k--)
                    cb[k] = cb[k+1]+cb1[k];
                }
                else
                {
                  cb[m-1]=cb2[m-1];
                  for(k=m-2; k>=0; k--)
                    cb[k] = cb[k+1]+cb2[k];
                }

                /// Compute DTW and early abandoning if possible
                dist = dtw(tz, q, cb, m, r, bsf);

                if( dist < bsf )
                {   /// Update bsf
                  /// loc is the real starting location of the nearest neighbor in the file
                  bsf = dist;
                  loc = (it)*(EPOCH-m+1) + i-m+1;
                }
              } else
                keogh2++;
            } else
              keogh++;
          } else
            kim++;

          /// Reduce obsolute points from sum and sum square
          ex -= t[j];
          ex2 -= t[j]*t[j];
        }
      }

      /// If the size of last chunk is less then EPOCH, then no more data and terminate.
      if (ep<EPOCH)
        done=true;
      else
        it++;
    }
  }

  i = (it)*(EPOCH-m+1) + ep;
  fclose(fp);

  free(q);
  free(u);
  free(l);
  free(uo);
  free(lo);
  free(qo);
  free(cb);
  free(cb1);
  free(cb2);
  free(tz);
  free(t);
  free(l_d);
  free(u_d);
  free(l_buff);
  free(u_buff);
  free(order);
  free(buffer);

  //t2 = clock();
  //Rprintf("\n");

  /// Note that loc and i are rcpp_long_long_type.
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("location") = loc + 1, //convert to R's 1-based indexing
                                      Rcpp::Named("distance") = sqrt(bsf),
                                      //Rcpp::Named("exec_time")=(t2-t1)/CLOCKS_PER_SEC,
                                      Rcpp::Named("prunedKim")=((double) kim / i)*100,
                                      Rcpp::Named("prunedKeogh")=((double) keogh / i)*100,
                                      Rcpp::Named("prunedKeogh2")=((double) keogh2 / i)*100,
                                      Rcpp::Named("dtwCalc")=100-(((double)kim+keogh+keogh2)/i*100));
  out.attr("class") = "ucrdtw";
  return out;
}

/// Just the main function adapted for Rcpp export
//' UCR DTW Algorithm vector-vector method
//'
//' Sliding-window similarity search using DTW distance. This implementation of the UCR Suite command line utility, takes an R numeric vector as data input and an R numeric vector for the query
//'
//' @name ucrdtw_vv
//' @param data numeric vector containing data
//' @param query numeric vector containing the query. The length of this vector determines the query length.
//' @param dtwwindow double; Size of the warping window size (as a proportion of query length). The DTW calculation in `rucrdtw` uses a symmetric Sakoe-Chiba band. See Giorgino (2009) for a general coverage of warping window constraints.
//' @param epoch int defaults to 1e5, should be \eqn{\le} 1e6. This is the size of the data chunk that is processed at once. All cumulative values in the algorithm will be restarted after \code{epoch} iterations to reduce floating point errors in these values.
//' @param skip bool defaults to FALSE. If TRUE bound calculations and if necessary, distance calculations, are only performed on non-overlapping segments of the data (i.e. multiples of \code{qlength}). This is useful if \code{data} is a set of multiple reference time series, each of length \code{qlength}. The location returned when skipping is the index of the subsequence.
//' @return a ucrdtw object. A list with the following elements
//' \itemize{
//'   \item \strong{location:} The starting location of the nearest neighbor of the given query, of size \code{length(query)}, in the data. Note that location starts from 1.
//'   \item \strong{distance:} The DTW distance between the nearest neighbor and the query.
//'   \item \strong{prunedKim:} Percentage of subsequences that were pruned based on the LB-Kim criterion.
//'   \item \strong{prunedKeogh:} Percentage of subsequences that were pruned based on the LB-Keogh-EQ criterion.
//'   \item \strong{prunedKeogh2:} Percentage of subsequences that were pruned based on the LB-Keogh-EC criterion.
//'   \item \strong{dtwCalc:} Percentage of subsequences for which the full DTW distance was calculated.
//' }
//' For an explanation of the pruning criteria see Rakthanmanon et al. (2012).
//' @references Rakthanmanon, Thanawin, Bilson Campana, Abdullah Mueen, Gustavo Batista, Brandon Westover, Qiang Zhu, Jesin Zakaria, and Eamonn Keogh. 2012. Searching and Mining Trillions of Time Series Subsequences Under Dynamic Time Warping. In Proceedings of the 18th ACM SIGKDD International Conference on Knowledge Discovery and Data Mining, 262-70. ACM. doi:\doi{10.1145/2339530.2339576}.
//' @references Giorgino, Toni (2009). Computing and Visualizing Dynamic Time Warping Alignments in R: The dtw Package. Journal of Statistical Software, 31(7), 1-24, doi:\doi{10.18637/jss.v031.i07}.
//' @examples
//' #read example data into vector
//' datav <- scan(system.file("extdata/col_sc.txt", package="rucrdtw"))
//' #read example query into vector
//' query <- scan(system.file("extdata/first_sc.txt", package="rucrdtw"))
//' #execute query
//' ucrdtw_vv(datav, query, 0.05)
//' @useDynLib rucrdtw, .registration=TRUE
//' @importFrom Rcpp sourceCpp
//' @export
// [[Rcpp::export]]
Rcpp::List ucrdtw_vv(Rcpp::NumericVector data , Rcpp::NumericVector query, double dtwwindow, int epoch = 100000, bool skip = false)
{
  //FILE *fp;            /// data file pointer
  //FILE *qp;            /// query file pointer
  double bsf;          /// best-so-far
  double *t, *q;       /// data array and query array
  int *order;          ///new order of the query
  double *u, *l, *qo, *uo, *lo,*tz,*cb, *cb1, *cb2,*u_d, *l_d;


  double d;
  rcpp_long_long_type i , j;
  double ex , ex2 , mean, std;
  int m=-1, r=-1;
  rcpp_long_long_type loc = 0, loc2 = 0;
  //double t1,t2;
  int kim = 0,keogh = 0, keogh2 = 0;
  double dist=0, lb_kim=0, lb_k=0, lb_k2=0;
  double *buffer, *u_buff, *l_buff;
  Index *Q_tmp;

  ///print size of data vector
  //Rcout << data.size() << "\n";


  /// For every EPOCH points, all cummulative values, such as ex (sum), ex2 (sum square), will be restarted for reducing the floating point error.
  int EPOCH = epoch;

  /// If not enough input, display an error.
  //if (argc<=3)
  //  error(4);

  /// read size of the query
  m = query.size();

  /// read warping windows
  {   double R = dtwwindow;
    if (R<=1)
      r = floor(R*m);
    else
      r = floor(R);
  }

  //fp = fopen(data,"r");
  //if( fp == NULL )
  //  error(2);

  //Rcpp::Rcout << "opened data file \n";
  //qp = fopen(query,"r");
  //if( qp == NULL )
  //error(2);

  /// start the clock
  //t1 = clock();


  /// malloc everything here
  q = (double *)malloc(sizeof(double)*m);
  if( q == NULL )
    error(1);
  qo = (double *)malloc(sizeof(double)*m);
  if( qo == NULL )
    error(1);
  uo = (double *)malloc(sizeof(double)*m);
  if( uo == NULL )
    error(1);
  lo = (double *)malloc(sizeof(double)*m);
  if( lo == NULL )
    error(1);

  order = (int *)malloc(sizeof(int)*m);
  if( order == NULL )
    error(1);

  Q_tmp = (Index *)malloc(sizeof(Index)*m);
  if( Q_tmp == NULL )
    error(1);

  u = (double *)malloc(sizeof(double)*m);
  if( u == NULL )
    error(1);

  l = (double *)malloc(sizeof(double)*m);
  if( l == NULL )
    error(1);

  cb = (double *)malloc(sizeof(double)*m);
  if( cb == NULL )
    error(1);

  cb1 = (double *)malloc(sizeof(double)*m);
  if( cb1 == NULL )
    error(1);

  cb2 = (double *)malloc(sizeof(double)*m);
  if( cb2 == NULL )
    error(1);

  u_d = (double *)malloc(sizeof(double)*m);
  if( u == NULL )
    error(1);

  l_d = (double *)malloc(sizeof(double)*m);
  if( l == NULL )
    error(1);

  t = (double *)malloc(sizeof(double)*m*2);
  if( t == NULL )
    error(1);

  tz = (double *)malloc(sizeof(double)*m);
  if( tz == NULL )
    error(1);

  buffer = (double *)malloc(sizeof(double)*EPOCH);
  if( buffer == NULL )
    error(1);

  u_buff = (double *)malloc(sizeof(double)*EPOCH);
  if( u_buff == NULL )
    error(1);

  l_buff = (double *)malloc(sizeof(double)*EPOCH);
  if( l_buff == NULL )
    error(1);


  /// Read query file
  bsf = INF;
  i = 0;
  j = 0;
  ex = ex2 = 0;

  //while(fscanf(qp,"%lf",&d) != EOF && i < m)
  //Rcpp::Rcout << "attempting to fill query\n";
  for (i = 0; i < m; i++)
  {
    ex += query[i];
    ex2 += query[i]*query[i];
    q[i] = query[i];
  }
  //Rcpp::Rcout << *q << "\n";
  //fclose(qp);

  /// Do z-normalize the query, keep in same array, q
  mean = ex/m;
  std = ex2/m;
  std = sqrt(std-mean*mean);
  for( i = 0 ; i < m ; i++ )
    q[i] = (q[i] - mean)/std;
  //Rcpp::Rcout << "done with normalisation\n";
  /// Create envelop of the query: lower envelop, l, and upper envelop, u
  lower_upper_lemire(q, m, r, l, u);

  /// Sort the query one time by abs(z-norm(q[i]))
  for( i = 0; i<m; i++)
  {
    Q_tmp[i].value = q[i];
    Q_tmp[i].index = i;
  }
  qsort(Q_tmp, m, sizeof(Index),comp);

  /// also create another arrays for keeping sorted envelop
  for( i=0; i<m; i++)
  {   int o = Q_tmp[i].index;
    order[i] = o;
    qo[i] = q[o];
    uo[i] = u[o];
    lo[i] = l[o];
  }
  free(Q_tmp);

  /// Initial the cummulative lower bound
  for( i=0; i<m; i++)
  {   cb[i]=0;
    cb1[i]=0;
    cb2[i]=0;
  }

  i = 0;          /// current index of the data in current chunk of size EPOCH
  j = 0;          /// the starting index of the data in the circular array, t
  ex = ex2 = 0;
  bool done = false;
  int it=0, ep=0, k=0;
  rcpp_long_long_type I;    /// the starting index of the data in current chunk of size EPOCH

  while(!done)
  {
    /// Read first m-1 points
    ep=0;
    if (it==0)
    {   for(k=0; k<m-1; k++)
      // if (fscanf(fp,"%lf",&d) != EOF)
      if (k < data.size()-1) //may break here
        buffer[k] = data[k];
    }
    else
    {   for(k=0; k<m-1; k++)
      buffer[k] = buffer[EPOCH-m+1+k];
    }

    /// Read buffer of size EPOCH or when all data has been read.
    ep=m-1;
    while(ep<EPOCH)
    {   //if (fscanf(fp,"%lf",&d) == EOF)
      if (((it)*(EPOCH-m+1) + ep) == data.size())
        break;
      buffer[ep] = data[(it)*(EPOCH-m+1) + ep];
      ep++;
      //Rcout << ep << "\n";
    }
    //Rcout << it << "\n";
    /// Data are read in chunk of size EPOCH.
    /// When there is nothing to read, the loop is end.
    if (ep<=m-1)
    {   done = true;
    } else
    {   lower_upper_lemire(buffer, ep, r, l_buff, u_buff);

      /// Just for printing a dot for approximate a million point. Not much accurate.
      //if (it%(1000000/(EPOCH-m+1))==0)
      //REprintf(".");
      ///check for user input in case of run-away loop
      Rcpp::checkUserInterrupt();
      /// Do main task here..
      ex=0; ex2=0;
      for(i=0; i<ep; i++)
      {
        /// A bunch of data has been read and pick one of them at a time to use
        d = buffer[i];

        /// Calcualte sum and sum square
        ex += d;
        ex2 += d*d;

        /// t is a circular array for keeping current data
        t[i%m] = d;

        /// Double the size for avoiding using modulo "%" operator
        t[(i%m)+m] = d;

        /// Start the task when there are more than m-1 points in the current chunk
        if( i >= m-1 )
        {
          mean = ex/m;
          std = ex2/m;
          std = sqrt(std-mean*mean);

          /// compute the start location of the data in the current circular array, t
          j = (i+1)%m;
          /// the start location of the data in the current chunk
          I = i-(m-1);

          //calculate current position in the data and only do bound calculations etc for non-overlapping sections of data
          if (skip)
            loc2 = (it)*(EPOCH-m+1) + i-m+1;
          if (!skip || (skip & (loc2 % m == 0))){

          /// Use a constant lower bound to prune the obvious subsequence
          lb_kim = lb_kim_hierarchy(t, q, j, m, mean, std, bsf);

          if (lb_kim < bsf)
          {
            /// Use a linear time lower bound to prune; z_normalization of t will be computed on the fly.
            /// uo, lo are envelop of the query.
            lb_k = lb_keogh_cumulative(order, t, uo, lo, cb1, j, m, mean, std, bsf);
            if (lb_k < bsf)
            {
              /// Take another linear time to compute z_normalization of t.
              /// Note that for better optimization, this can merge to the previous function.
              for(k=0;k<m;k++)
              {   tz[k] = (t[(k+j)] - mean)/std;
              }

              /// Use another lb_keogh to prune
              /// qo is the sorted query. tz is unsorted z_normalized data.
              /// l_buff, u_buff are big envelop for all data in this chunk
              lb_k2 = lb_keogh_data_cumulative(order, tz, qo, cb2, l_buff+I, u_buff+I, m, mean, std, bsf);
              if (lb_k2 < bsf)
              {
                /// Choose better lower bound between lb_keogh and lb_keogh2 to be used in early abandoning DTW
                /// Note that cb and cb2 will be cumulative summed here.
                if (lb_k > lb_k2)
                {
                  cb[m-1]=cb1[m-1];
                  for(k=m-2; k>=0; k--)
                    cb[k] = cb[k+1]+cb1[k];
                }
                else
                {
                  cb[m-1]=cb2[m-1];
                  for(k=m-2; k>=0; k--)
                    cb[k] = cb[k+1]+cb2[k];
                }

                /// Compute DTW and early abandoning if possible
                dist = dtw(tz, q, cb, m, r, bsf);

                if( dist < bsf )
                {   /// Update bsf
                  /// loc is the real starting location of the nearest neighbor in the file
                  bsf = dist;
                  loc = (it)*(EPOCH-m+1) + i-m+1;
                }
              } else
                keogh2++;
            } else
              keogh++;
          } else
            kim++;
          }

          /// Reduce obsolute points from sum and sum square
          ex -= t[j];
          ex2 -= t[j]*t[j];
        }
      }

      /// If the size of last chunk is less then EPOCH, then no more data and terminate.
      if (ep<EPOCH)
        done=true;
      else
        it++;
    }
  }

  i = (it)*(EPOCH-m+1) + ep;
  //fclose(fp);

  free(q);
  free(u);
  free(l);
  free(uo);
  free(lo);
  free(qo);
  free(cb);
  free(cb1);
  free(cb2);
  free(tz);
  free(t);
  free(l_d);
  free(u_d);
  free(l_buff);
  free(u_buff);
  free(order);
  free(buffer);

  //t2 = clock();
  //Rprintf("\n");

  /// Note that loc and i are rcpp_long_long_type.
  // convert location to
  Rcpp::List out = Rcpp::List::create(Rcpp::Named("location") = (skip) ? (loc / m + 1) : (loc + 1), //loc is index of subsequence if skipping, otherwise convert raw data location to R's 1-based indexing
                                      Rcpp::Named("distance") = sqrt(bsf),
                                      //Rcpp::Named("exec_time")=(t2-t1)/CLOCKS_PER_SEC,
                                      Rcpp::Named("prunedKim")=((double) kim / i)*100,
                                      Rcpp::Named("prunedKeogh")=((double) keogh / i)*100,
                                      Rcpp::Named("prunedKeogh2")=((double) keogh2 / i)*100,
                                      Rcpp::Named("dtwCalc")=100-(((double)kim+keogh+keogh2)/i*100));
  out.attr("class") = "ucrdtw";
  return out;
}


