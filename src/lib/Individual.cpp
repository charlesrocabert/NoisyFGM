
/**
 * \file      Individual.cpp
 * \authors   Charles Rocabert, Samuel Bernard, Carole Knibbe, Guillaume Beslon
 * \date      28-03-2018
 * \copyright Copyright (C) 2016-2018 Charles Rocabert, Samuel Bernard, Carole Knibbe, Guillaume Beslon. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Individual class definition
 */

/***********************************************************************
 * Copyright (C) 2016-2018
 * Charles Rocabert, Samuel Bernard, Carole Knibbe, Guillaume Beslon
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************/

#include "Individual.h"


/*----------------------------
 * CONSTRUCTORS
 *----------------------------*/

/**
 * \brief    Constructor
 * \details  --
 * \param    Prng* prng
 * \param    int n
 * \param    double mu_init
 * \param    double sigma_init
 * \param    double theta_init
 * \param    bool oneD_shift
 * \param    bool type_of_noise noise_type
 * \param    gsl_vector* z_opt
 * \return   \e void
 */
Individual::Individual( Prng* prng, int n, double mu_init, double sigma_init, double theta_init, bool oneD_shift, type_of_noise noise_type, gsl_vector* z_opt )
{
  /*----------------------------------------------- PARAMETERS */
  
  _prng       = prng;
  _n          = n;
  _noise_type = noise_type;
  _z_opt      = z_opt;
  
  /*----------------------------------------------- VARIABLES */
  
  _identifier = 0;
  _generation = 0;
  
  /******************************/
  /* Initialize matrices        */
  /******************************/
  _Sigma    = NULL;
  _Cholesky = NULL;
  
  /******************************/
  /* Initialize mu              */
  /******************************/
  _mu = gsl_vector_alloc(_n);
  if (oneD_shift)
  {
    gsl_vector_set_zero(_mu);
    gsl_vector_set(_mu, 0, mu_init);
  }
  else
  {
    gsl_vector_set_all(_mu, mu_init);
  }
  
  /******************************/
  /* Initialize sigma           */
  /******************************/
  _sigma = NULL;
  if (_noise_type != NONE)
  {
    _sigma = gsl_vector_alloc(_n);
    gsl_vector_set_all(_sigma, sigma_init);
  }
  
  /******************************/
  /* Initialize theta           */
  /******************************/
  _theta = NULL;
  if (_n > 1 && _noise_type == FULL)
  {
    _theta = gsl_vector_alloc(_n*(_n-1)/2);
    gsl_vector_set_all(_theta, theta_init);
  }
  
  /******************************/
  /* Initialize z               */
  /******************************/
  _z = gsl_vector_alloc(_n);
  gsl_vector_set_zero(_z);
  
  /******************************/
  /* Initialize other variables */
  /******************************/
  _dg = 0.0;
  _dp = 0.0;
  _wg = 0.0;
  _wp = 0.0;
  
  /*----------------------------------------------- MAPPING PROPERTIES */
  
  _phenotype_is_built     = false;
  _max_Sigma_eigenvector  = NULL;
  _max_Sigma_eigenvalue   = 0.0;
  _max_Sigma_contribution = 0.0;
  _max_dot_product        = 0.0;
  
  /*----------------------------------------------- MUTATIONS */
  
  _r_mu    = 0.0;
  _r_sigma = 0.0;
  _r_theta = 0.0;
}

/**
 * \brief    Copy constructor
 * \details  --
 * \param    const Individual& individual
 * \return   \e void
 */
Individual::Individual( const Individual& individual )
{
  /*----------------------------------------------- PARAMETERS */
  
  _prng       = individual._prng;
  _n          = individual._n;
  _noise_type = individual._noise_type;
  _z_opt      = individual._z_opt;
  
  /*----------------------------------------------- VARIABLES */
  
  _identifier = individual._identifier;
  _generation = individual._generation;
  
  /******************************/
  /* Initialize matrices        */
  /******************************/
  _Sigma    = NULL;
  _Cholesky = NULL;
  
  /******************************/
  /* Initialize mu              */
  /******************************/
  _mu = gsl_vector_alloc(_n);
  gsl_vector_memcpy(_mu, individual._mu);
  
  /******************************/
  /* Initialize sigma           */
  /******************************/
  _sigma = NULL;
  if (_noise_type != NONE)
  {
    _sigma = gsl_vector_alloc(_n);
    gsl_vector_memcpy(_sigma, individual._sigma);
  }
  
  /******************************/
  /* Initialize theta           */
  /******************************/
  _theta = NULL;
  if (_n > 1 && _noise_type == FULL)
  {
    _theta = gsl_vector_alloc(_n*(_n-1)/2);
    gsl_vector_memcpy(_theta, individual._theta);
  }
  
  /******************************/
  /* Initialize z               */
  /******************************/
  _z = gsl_vector_alloc(_n);
  gsl_vector_memcpy(_z, individual._z);
  
  /******************************/
  /* Initialize other variables */
  /******************************/
  _dg = individual._dg;
  _dp = individual._dp;
  _wg = individual._wg;
  _wp = individual._wp;
  
  /*----------------------------------------------- MAPPING PROPERTIES */
  
  _phenotype_is_built     = false;
  _max_Sigma_eigenvector  = NULL;
  _max_Sigma_eigenvalue   = individual._max_Sigma_eigenvalue;
  _max_Sigma_contribution = individual._max_Sigma_contribution;
  _max_dot_product        = individual._max_dot_product;
  
  /*----------------------------------------------- MUTATIONS */
  
  _r_mu    = individual._r_mu;
  _r_sigma = individual._r_sigma;
  _r_theta = individual._r_theta;
}

/*----------------------------
 * DESTRUCTORS
 *----------------------------*/

/**
 * \brief    Destructor
 * \details  --
 * \param    void
 * \return   \e void
 */
Individual::~Individual( void )
{
  _prng  = NULL;
  _z_opt = NULL;
  gsl_vector_free(_mu);
  _mu = NULL;
  if (_noise_type != NONE)
  {
    gsl_vector_free(_sigma);
    _sigma = NULL;
    gsl_matrix_free(_Sigma);
    _Sigma = NULL;
    gsl_matrix_free(_Cholesky);
    _Cholesky = NULL;
    gsl_vector_free(_max_Sigma_eigenvector);
    _max_Sigma_eigenvector = NULL;
    if (_n > 1 && _noise_type == FULL)
    {
      gsl_vector_free(_theta);
      _theta = NULL;
    }
  }
  gsl_vector_free(_z);
  _z = NULL;
}

/*----------------------------
 * PUBLIC METHODS
 *----------------------------*/

/**
 * \brief    Mutate the individual genotype
 * \details  --
 * \param    double m_mu
 * \param    double m_sigma
 * \param    double m_theta
 * \param    double s_mu
 * \param    double s_sigma
 * \param    double s_theta
 * \return   \e void
 */
void Individual::mutate( double m_mu, double m_sigma, double m_theta, double s_mu, double s_sigma, double s_theta )
{
  gsl_vector* previous_mu    = NULL;
  gsl_vector* previous_sigma = NULL;
  gsl_vector* previous_theta = NULL;
  
  /*****************************/
  /* 1) Save current genotype  */
  /*****************************/
  previous_mu = gsl_vector_alloc(_n);
  gsl_vector_memcpy(previous_mu, _mu);
  if (_noise_type != NONE)
  {
    previous_sigma = gsl_vector_alloc(_n);
    gsl_vector_memcpy(previous_sigma, _sigma);
    if (_n > 1 && _noise_type == FULL)
    {
      previous_theta = gsl_vector_alloc(_n*(_n-1)/2);
      gsl_vector_memcpy(previous_theta, _theta);
    }
  }
  
  /*****************************/
  /* 2) Mutate mu vector       */
  /*****************************/
  if (_prng->uniform() < m_mu)
  {
    for (int i = 0; i < _n; i++)
    {
      gsl_vector_set(_mu, i, gsl_vector_get(_mu, i)+_prng->gaussian(0.0, s_mu));
    }
    _phenotype_is_built = false;
  }
  
  /*****************************/
  /* 3) Mutate sigma vector    */
  /*****************************/
  if (_noise_type != NONE && _prng->uniform() < m_sigma)
  {
    if (_noise_type == ISOTROPIC)
    {
      double new_sigma = fabs(gsl_vector_get(_sigma, 0)+_prng->gaussian(0.0, s_sigma));
      gsl_vector_set_all(_sigma, new_sigma);
    }
    else if (_noise_type == UNCORRELATED || _noise_type == FULL)
    {
      for (int i = 0; i < _n; i++)
      {
        gsl_vector_set(_sigma, i, fabs(gsl_vector_get(_sigma, i)+_prng->gaussian(0.0, s_sigma)));
      }
    }
    _phenotype_is_built = false;
  }
  
  /*****************************/
  /* 4) Mutate theta vector    */
  /*****************************/
  if (_n > 1 && _noise_type == FULL && _prng->uniform() < m_theta)
  {
    for (int i = 0; i < _n*(_n-1)/2; i++)
    {
      gsl_vector_set(_theta, i, gsl_vector_get(_theta, i)+_prng->gaussian(0.0, s_theta));
    }
  }
  _phenotype_is_built = false;
  
  /*****************************/
  /* 5) Compute mutation sizes */
  /*****************************/
  _r_mu    = 0.0;
  _r_sigma = 0.0;
  _r_theta = 0.0;
  for (int i = 0; i < _n; i++)
  {
    double p_mu = gsl_vector_get(previous_mu, i);
    double mu   = gsl_vector_get(_mu, i);
    _r_mu      += (mu-p_mu)*(mu-p_mu);
    if (_noise_type != NONE)
    {
      double p_sigma = gsl_vector_get(previous_sigma, i);
      double sigma   = gsl_vector_get(_sigma, i);
      _r_sigma      += (sigma-p_sigma)*(sigma-p_sigma);
    }
  }
  if (_n > 1 && _noise_type == FULL)
  {
    for (int i = 0; i < _n*(_n-1)/2; i++)
    {
      double p_theta  = gsl_vector_get(previous_theta, i);
      double theta    = gsl_vector_get(_theta, i);
      _r_theta       += (theta-p_theta)*(theta-p_theta);
    }
  }
  
  _r_mu    = sqrt(_r_mu);
  _r_sigma = sqrt(_r_sigma);
  _r_theta = sqrt(_r_theta);
  gsl_vector_free(previous_mu);
  previous_mu = NULL;
  gsl_vector_free(previous_sigma);
  previous_sigma = NULL;
  gsl_vector_free(previous_theta);
  previous_theta = NULL;
}

/**
 * \brief    Build the phenotype
 * \details  If the mapping is not built, build it and draw z. Else just draw z
 * \param    void
 * \return   \e void
 */
void Individual::build_phenotype( void )
{
  if (!_phenotype_is_built)
  {
    if (_noise_type != NONE)
    {
      build_Sigma();
      compute_dot_product();
      Cholesky_decomposition();
      clear_memory();
    }
    _phenotype_is_built = true;
  }
  draw_z();
}

/**
 * \brief    Compute the fitness
 * \details  --
 * \param    double alpha
 * \param    double beta
 * \param    double Q
 * \return   \e void
 */
void Individual::compute_fitness( double alpha, double beta, double Q )
{
  _dg = 0.0;
  _dp = 0.0;
  for (int i = 0; i < _n; i++)
  {
    double mu_diff  = gsl_vector_get(_mu, i)-gsl_vector_get(_z_opt, i);
    double z_diff   = gsl_vector_get(_z, i)-gsl_vector_get(_z_opt, i);
    _dg            += mu_diff*mu_diff;
    _dp            += z_diff*z_diff;
  }
  _dg = sqrt(_dg);
  _dp = sqrt(_dp);
  _wg = exp(-(1.0-beta)*alpha*pow(_dg, Q)+beta);
  _wp = exp(-(1.0-beta)*alpha*pow(_dp, Q)+beta);
}

/*----------------------------
 * PROTECTED METHODS
 *----------------------------*/

/**
 * \brief    Rotate the matrix m by angle theta on the plane (a, b)
 * \details  --
 * \param    int a
 * \param    int b
 * \param    double theta
 * \return   \e void
 */
void Individual::rotate( gsl_matrix* m, int a, int b, double theta )
{
  gsl_matrix* newm = gsl_matrix_alloc(_n, _n);
  gsl_matrix_memcpy(newm, m);
  for (int k = 0; k < _n; k++)
  {
    gsl_matrix_set(newm, a, k, cos(theta)*gsl_matrix_get(m, a, k)-sin(theta)*gsl_matrix_get(m, b, k));
    gsl_matrix_set(newm, b, k, sin(theta)*gsl_matrix_get(m, a, k)+cos(theta)*gsl_matrix_get(m, b, k));
  }
  gsl_matrix_memcpy(m, newm);
  gsl_matrix_free(newm);
  newm = NULL;
}

/**
 * \brief    Build the co-variance matrix Sigma
 * \details  --
 * \param    void
 * \return   \e void
 */
void Individual::build_Sigma( void )
{
  /*****************************************/
  /* 1) Create eigenvectors matrix         */
  /*****************************************/
  gsl_matrix* X = gsl_matrix_alloc(_n, _n);
  gsl_matrix_set_identity(X);
  
  /*****************************************/
  /* 2) Starting from identity matrix,     */
  /*    apply the n(n-1)/2 rotations to    */
  /*    the eigenvectors                   */
  /*****************************************/
  if (_n > 1 && _noise_type == FULL)
  {
    int counter = 0;
    for (int a = 0; a < _n; a++)
    {
      for (int b = a+1; b < _n; b++)
      {
        rotate(X, a, b, gsl_vector_get(_theta, counter));
        counter++;
      }
    }
    assert(counter == _n*(_n-1)/2);
  }
  
  /*****************************************/
  /* 3) Create the matrix D of eigenvalues */
  /*****************************************/
  _max_Sigma_eigenvalue = 0.0;
  int max_sigma_index   = 0;
  double sigma_sum      = 0.0;
  gsl_matrix* D = gsl_matrix_alloc(_n, _n);
  gsl_matrix_set_zero(D);
  for (int i = 0; i < _n; i++)
  {
    double sigma = gsl_vector_get(_sigma, i);
    gsl_matrix_set(D, i, i, sigma);
    sigma_sum += sigma;
    if (_max_Sigma_eigenvalue < sigma)
    {
      _max_Sigma_eigenvalue = sigma;
      max_sigma_index       = i;
    }
  }
  
  /*****************************************/
  /* 4) Save maximum eigenvector and       */
  /*    eigenvalue contribution            */
  /*****************************************/
  _max_Sigma_eigenvector  = gsl_vector_alloc(_n);
  _max_Sigma_contribution = _max_Sigma_eigenvalue/sigma_sum;
  for (int i = 0; i < _n; i++)
  {
    gsl_vector_set(_max_Sigma_eigenvector, i, gsl_matrix_get(X, i, max_sigma_index));
  }
  
  /*****************************************/
  /* 5) Compute Sigma = X * D * X^-1       */
  /*****************************************/
  gsl_matrix* P = gsl_matrix_alloc(_n, _n);
  _Sigma        = gsl_matrix_alloc(_n, _n);
  
  gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1.0, D, X, 0.0, P);
  gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, X, P, 0.0, _Sigma);
  gsl_matrix_free(X);
  X = NULL;
  gsl_matrix_free(D);
  D = NULL;
  gsl_matrix_free(P);
  P = NULL;
}

/**
 * \brief    Compute the dot product between Sigma eigen vector and optimum direction
 * \details  --
 * \param    void
 * \return   \e void
 */
void Individual::compute_dot_product( void )
{
  _max_dot_product = 0.0;
  gsl_vector* d    = gsl_vector_alloc(_n);
  gsl_vector_memcpy(d, _z_opt);
  gsl_vector_sub(d, _mu);
  double norm = gsl_blas_dnrm2(d);
  for (int i = 0; i < _n; i++)
  {
    gsl_vector_set(d, i, gsl_vector_get(d, i)/norm);
  }
  gsl_blas_ddot(d, _max_Sigma_eigenvector, &_max_dot_product);
  gsl_vector_free(d);
  d = NULL;
  _max_dot_product = fabs(_max_dot_product);
}

/**
 * \brief    Compute cholesky decomposition
 * \details  --
 * \param    void
 * \return   \e void
 */
void Individual::Cholesky_decomposition( void )
{
  gsl_matrix_free(_Cholesky);
  _Cholesky = NULL;
  _Cholesky = gsl_matrix_alloc(_n, _n);
  gsl_matrix_memcpy(_Cholesky, _Sigma);
  gsl_linalg_cholesky_decomp(_Cholesky);
  /* L est dans le triangle inférieur */
}

/**
 * \brief    Draw the phenotype z in a multivariate normal law N(_mu, _Sigma)
 * \details  In details, we apply the cholesky decomposition method to transform centered-reduced normal points.
 * \param    void
 * \return   \e void
 */
void Individual::draw_z( void )
{
  if (_noise_type == NONE)
  {
    /* Copy mu vector in z vector */
    gsl_vector_memcpy(_z, _mu);
  }
  else
  {
    /* Draw the uniform vector N(0,1) */
    for (int i = 0; i < _n; i++)
    {
      gsl_vector_set(_z, i, _prng->gaussian(0.0, 1.0));
    }
    
    /* Apply cholesky matrix */
    gsl_blas_dtrmv(CblasLower, CblasNoTrans, CblasNonUnit, _Cholesky, _z);
    gsl_vector_add(_z, _mu);
  }
}

/**
 * \brief    Clear the memory
 * \details  --
 * \param    void
 * \return   \e void
 */
void Individual::clear_memory( void )
{
  gsl_matrix_free(_Sigma);
  _Sigma = NULL;
  gsl_vector_free(_max_Sigma_eigenvector);
  _max_Sigma_eigenvector = NULL;
}

/**
 * \brief    Delete all vectors and matrices
 * \details  --
 * \param    void
 * \return   \e void
 */
void Individual::delete_vectors_and_matrices( void )
{
  gsl_vector_free(_mu);
  _mu = NULL;
  if (_noise_type != NONE)
  {
    gsl_vector_free(_sigma);
    _sigma = NULL;
    gsl_matrix_free(_Sigma);
    _Sigma = NULL;
    gsl_matrix_free(_Cholesky);
    _Cholesky = NULL;
    gsl_vector_free(_max_Sigma_eigenvector);
    _max_Sigma_eigenvector = NULL;
    if (_n > 1 && _noise_type == FULL)
    {
      gsl_vector_free(_theta);
      _theta = NULL;
    }
  }
  gsl_vector_free(_z);
  _z = NULL;
}

