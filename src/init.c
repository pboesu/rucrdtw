#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* .Call calls */
extern SEXP rucrdtw_RcppExport_registerCCallable();
extern SEXP rucrdtw_ucrdtw_ff(SEXP, SEXP, SEXP, SEXP);
extern SEXP rucrdtw_ucrdtw_fv(SEXP, SEXP, SEXP);
extern SEXP rucrdtw_ucrdtw_vv(SEXP, SEXP, SEXP, SEXP, SEXP);
extern SEXP rucrdtw_ucred_ff(SEXP, SEXP, SEXP);
extern SEXP rucrdtw_ucred_fv(SEXP, SEXP);
extern SEXP rucrdtw_ucred_vv(SEXP, SEXP, SEXP);

static const R_CallMethodDef CallEntries[] = {
  {"rucrdtw_RcppExport_registerCCallable", (DL_FUNC) &rucrdtw_RcppExport_registerCCallable, 0},
  {"rucrdtw_ucrdtw_ff",                    (DL_FUNC) &rucrdtw_ucrdtw_ff,                    4},
  {"rucrdtw_ucrdtw_fv",                    (DL_FUNC) &rucrdtw_ucrdtw_fv,                    3},
  {"rucrdtw_ucrdtw_vv",                    (DL_FUNC) &rucrdtw_ucrdtw_vv,                    5},
  {"rucrdtw_ucred_ff",                     (DL_FUNC) &rucrdtw_ucred_ff,                     3},
  {"rucrdtw_ucred_fv",                     (DL_FUNC) &rucrdtw_ucred_fv,                     2},
  {"rucrdtw_ucred_vv",                     (DL_FUNC) &rucrdtw_ucred_vv,                     3},
  {NULL, NULL, 0}
};

void R_init_rucrdtw(DllInfo *dll)
{
  R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
  R_useDynamicSymbols(dll, FALSE);
}
