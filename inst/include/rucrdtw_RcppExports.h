// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#ifndef RCPP_rucrdtw_RCPPEXPORTS_H_GEN_
#define RCPP_rucrdtw_RCPPEXPORTS_H_GEN_

#include <Rcpp.h>

namespace rucrdtw {

    using namespace Rcpp;

    namespace {
        void validateSignature(const char* sig) {
            Rcpp::Function require = Rcpp::Environment::base_env()["require"];
            require("rucrdtw", Rcpp::Named("quietly") = true);
            typedef int(*Ptr_validate)(const char*);
            static Ptr_validate p_validate = (Ptr_validate)
                R_GetCCallable("rucrdtw", "_rucrdtw_RcppExport_validate");
            if (!p_validate(sig)) {
                throw Rcpp::function_not_exported(
                    "C++ function with signature '" + std::string(sig) + "' not found in rucrdtw");
            }
        }
    }

    inline Rcpp::List ucrdtw_ff(const char * data, const char * query, int qlength, double dtwwindow) {
        typedef SEXP(*Ptr_ucrdtw_ff)(SEXP,SEXP,SEXP,SEXP);
        static Ptr_ucrdtw_ff p_ucrdtw_ff = NULL;
        if (p_ucrdtw_ff == NULL) {
            validateSignature("Rcpp::List(*ucrdtw_ff)(const char *,const char *,int,double)");
            p_ucrdtw_ff = (Ptr_ucrdtw_ff)R_GetCCallable("rucrdtw", "_rucrdtw_ucrdtw_ff");
        }
        RObject rcpp_result_gen;
        {
            RNGScope RCPP_rngScope_gen;
            rcpp_result_gen = p_ucrdtw_ff(Shield<SEXP>(Rcpp::wrap(data)), Shield<SEXP>(Rcpp::wrap(query)), Shield<SEXP>(Rcpp::wrap(qlength)), Shield<SEXP>(Rcpp::wrap(dtwwindow)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::List >(rcpp_result_gen);
    }

    inline Rcpp::List ucrdtw_fv(const char * data, Rcpp::NumericVector query, double dtwwindow) {
        typedef SEXP(*Ptr_ucrdtw_fv)(SEXP,SEXP,SEXP);
        static Ptr_ucrdtw_fv p_ucrdtw_fv = NULL;
        if (p_ucrdtw_fv == NULL) {
            validateSignature("Rcpp::List(*ucrdtw_fv)(const char *,Rcpp::NumericVector,double)");
            p_ucrdtw_fv = (Ptr_ucrdtw_fv)R_GetCCallable("rucrdtw", "_rucrdtw_ucrdtw_fv");
        }
        RObject rcpp_result_gen;
        {
            RNGScope RCPP_rngScope_gen;
            rcpp_result_gen = p_ucrdtw_fv(Shield<SEXP>(Rcpp::wrap(data)), Shield<SEXP>(Rcpp::wrap(query)), Shield<SEXP>(Rcpp::wrap(dtwwindow)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::List >(rcpp_result_gen);
    }

    inline Rcpp::List ucrdtw_vv(Rcpp::NumericVector data, Rcpp::NumericVector query, double dtwwindow, int epoch = 100000, bool skip = false) {
        typedef SEXP(*Ptr_ucrdtw_vv)(SEXP,SEXP,SEXP,SEXP,SEXP);
        static Ptr_ucrdtw_vv p_ucrdtw_vv = NULL;
        if (p_ucrdtw_vv == NULL) {
            validateSignature("Rcpp::List(*ucrdtw_vv)(Rcpp::NumericVector,Rcpp::NumericVector,double,int,bool)");
            p_ucrdtw_vv = (Ptr_ucrdtw_vv)R_GetCCallable("rucrdtw", "_rucrdtw_ucrdtw_vv");
        }
        RObject rcpp_result_gen;
        {
            RNGScope RCPP_rngScope_gen;
            rcpp_result_gen = p_ucrdtw_vv(Shield<SEXP>(Rcpp::wrap(data)), Shield<SEXP>(Rcpp::wrap(query)), Shield<SEXP>(Rcpp::wrap(dtwwindow)), Shield<SEXP>(Rcpp::wrap(epoch)), Shield<SEXP>(Rcpp::wrap(skip)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::List >(rcpp_result_gen);
    }

    inline Rcpp::List ucred_ff(const char * data, const char * query, int qlength) {
        typedef SEXP(*Ptr_ucred_ff)(SEXP,SEXP,SEXP);
        static Ptr_ucred_ff p_ucred_ff = NULL;
        if (p_ucred_ff == NULL) {
            validateSignature("Rcpp::List(*ucred_ff)(const char *,const char *,int)");
            p_ucred_ff = (Ptr_ucred_ff)R_GetCCallable("rucrdtw", "_rucrdtw_ucred_ff");
        }
        RObject rcpp_result_gen;
        {
            RNGScope RCPP_rngScope_gen;
            rcpp_result_gen = p_ucred_ff(Shield<SEXP>(Rcpp::wrap(data)), Shield<SEXP>(Rcpp::wrap(query)), Shield<SEXP>(Rcpp::wrap(qlength)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::List >(rcpp_result_gen);
    }

    inline Rcpp::List ucred_fv(const char * data, Rcpp::NumericVector query) {
        typedef SEXP(*Ptr_ucred_fv)(SEXP,SEXP);
        static Ptr_ucred_fv p_ucred_fv = NULL;
        if (p_ucred_fv == NULL) {
            validateSignature("Rcpp::List(*ucred_fv)(const char *,Rcpp::NumericVector)");
            p_ucred_fv = (Ptr_ucred_fv)R_GetCCallable("rucrdtw", "_rucrdtw_ucred_fv");
        }
        RObject rcpp_result_gen;
        {
            RNGScope RCPP_rngScope_gen;
            rcpp_result_gen = p_ucred_fv(Shield<SEXP>(Rcpp::wrap(data)), Shield<SEXP>(Rcpp::wrap(query)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::List >(rcpp_result_gen);
    }

    inline Rcpp::List ucred_vv(Rcpp::NumericVector data, Rcpp::NumericVector query, bool skip = false) {
        typedef SEXP(*Ptr_ucred_vv)(SEXP,SEXP,SEXP);
        static Ptr_ucred_vv p_ucred_vv = NULL;
        if (p_ucred_vv == NULL) {
            validateSignature("Rcpp::List(*ucred_vv)(Rcpp::NumericVector,Rcpp::NumericVector,bool)");
            p_ucred_vv = (Ptr_ucred_vv)R_GetCCallable("rucrdtw", "_rucrdtw_ucred_vv");
        }
        RObject rcpp_result_gen;
        {
            RNGScope RCPP_rngScope_gen;
            rcpp_result_gen = p_ucred_vv(Shield<SEXP>(Rcpp::wrap(data)), Shield<SEXP>(Rcpp::wrap(query)), Shield<SEXP>(Rcpp::wrap(skip)));
        }
        if (rcpp_result_gen.inherits("interrupted-error"))
            throw Rcpp::internal::InterruptedException();
        if (Rcpp::internal::isLongjumpSentinel(rcpp_result_gen))
            throw Rcpp::LongjumpException(rcpp_result_gen);
        if (rcpp_result_gen.inherits("try-error"))
            throw Rcpp::exception(Rcpp::as<std::string>(rcpp_result_gen).c_str());
        return Rcpp::as<Rcpp::List >(rcpp_result_gen);
    }

}

#endif // RCPP_rucrdtw_RCPPEXPORTS_H_GEN_
