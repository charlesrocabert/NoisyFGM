
/**
 * \file      Individual.h
 * \authors   Charles Rocabert, Samuel Bernard, Carole Knibbe, Guillaume Beslon
 * \date      07-06-2016
 * \copyright Copyright (C) 2016-2018 Charles Rocabert, Samuel Bernard, Carole Knibbe, Guillaume Beslon. All rights reserved
 * \license   This project is released under the GNU General Public License
 * \brief     Individual class declaration
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

#ifndef __SigmaFGM__Individual__
#define __SigmaFGM__Individual__

#include <iostream>
#include <cmath>
#include <gsl/gsl_math.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_integration.h>
#include <gsl/gsl_randist.h>
#include <assert.h>

#include "Macros.h"
#include "Enums.h"
#include "Prng.h"


class Individual
{
  
public:
  
  /*----------------------------
   * CONSTRUCTORS
   *----------------------------*/
  Individual( void ) = delete;
  Individual( Prng* prng, size_t n, double m_mu, double m_sigma, double m_theta, double s_mu, double s_sigma, double s_theta, double mu_init, double sigma_init, double theta_init, bool oneD_shift, type_of_noise noise_type );
  Individual( const Individual& individual );
  
  /*----------------------------
   * DESTRUCTORS
   *----------------------------*/
  ~Individual( void );
  
  /*----------------------------
   * GETTERS
   *----------------------------*/
  
  /*----------------------------------------------- VARIABLES */
  
  inline double get_mu( size_t i ) const;
  inline double get_sigma( size_t i ) const;
  inline double get_theta( size_t i ) const;
  inline double get_dg( void ) const;
  inline double get_dp( void ) const;
  inline double get_wg( void ) const;
  inline double get_wp( void ) const;
  
  /*----------------------------------------------- MAPPING PROPERTIES */
  
  inline double get_max_Sigma_eigenvalue( void ) const;
  inline double get_max_Sigma_contribution( void ) const;
  inline double get_max_dot_product( void ) const;
  
  /*----------------------------------------------- MUTATIONS */
  
  inline double get_r_mu( void ) const;
  inline double get_r_sigma( void ) const;
  inline double get_r_theta( void ) const;
  
  /*----------------------------
   * SETTERS
   *----------------------------*/
  Individual& operator=(const Individual&) = delete;
  
  /*----------------------------
   * PUBLIC METHODS
   *----------------------------*/
  void jump( void );
  void build_phenotype( void );
  void compute_fitness( gsl_vector* z_opt, double alpha, double beta, double Q );
  
  /*----------------------------
   * PUBLIC ATTRIBUTES
   *----------------------------*/
  
protected:
  
  /*----------------------------
   * PROTECTED METHODS
   *----------------------------*/
  void rotate( gsl_matrix* m, size_t a, size_t b, double theta );
  void build_Sigma( void );
  void compute_dot_product( void );
  void Cholesky_decomposition( void );
  void draw_z( void );
  void clear_memory( void );
  
  /*----------------------------
   * PROTECTED ATTRIBUTES
   *----------------------------*/
  
  /*----------------------------------------------- PARAMETERS */
  
  Prng*         _prng;       /*!< Pseudorandom numbers generator  */
  int           _n;          /*!< Number of dimensions            */
  double        _m_mu;       /*!< Mu mutation rate                */
  double        _m_sigma;    /*!< Sigma mutation rate             */
  double        _m_theta;    /*!< Theta mutation rate             */
  double        _s_mu;       /*!< Mu mutation size                */
  double        _s_sigma;    /*!< Sigma mutation size             */
  double        _s_theta;    /*!< Theta mutation size             */
  type_of_noise _noise_type; /*!< Phenotypic noise properties     */
  
  /*----------------------------------------------- VARIABLES */
  
  gsl_vector* _mu;       /*!< Mu vector                     */
  gsl_vector* _sigma;    /*!< Sigma vector                  */
  gsl_vector* _theta;    /*!< Theta vector                  */
  gsl_matrix* _Sigma;    /*!< Co-variance matrix            */
  gsl_matrix* _Cholesky; /*!< Cholesky decomposition matrix */
  gsl_vector* _z;        /*!< Instantaneous phenotype       */
  double      _dg;       /*!< Euclidean distance d(mu)      */
  double      _dp;       /*!< Euclidean distance d(z)       */
  double      _wg;       /*!< Fitness w(mu)                 */
  double      _wp;       /*!< Fitness w(z)                  */
  
  /*----------------------------------------------- MAPPING PROPERTIES */
  
  bool        _phenotype_is_built;     /*!< Indicates if the phenotype is built                             */
  gsl_vector* _max_Sigma_eigenvector;  /*!< Eigen vector corresponding to the maximum variance of Sigma     */
  double      _max_Sigma_eigenvalue;   /*!< Eigen value corresponding to the maximum variance of Sigma      */
  double      _max_Sigma_contribution; /*!< Eigen value contribution to the total variance                  */
  double      _max_dot_product;        /*!< Dot product of maximum Sigma eigen vector and optimum direction */
  
  /*----------------------------------------------- MUTATIONS */
  
  double _r_mu;    /*!< Euclidean size of Mu mutation    */
  double _r_sigma; /*!< Euclidean size of Sigma mutation */
  double _r_theta; /*!< Euclidean size of Theta mutation */
  
};


/*----------------------------
 * GETTERS
 *----------------------------*/

/*----------------------------------------------- VARIABLES */

/**
 * \brief    Get mu value at position i
 * \details  --
 * \param    size_t i
 * \return   \e double
 */
inline double Individual::get_mu( size_t i ) const
{
  assert(i < _n);
  return gsl_vector_get(_mu, i);
}

/**
 * \brief    Get sigma value at position i
 * \details  --
 * \param    size_t i
 * \return   \e double
 */
inline double Individual::get_sigma( size_t i ) const
{
  assert(i < _n);
  return gsl_vector_get(_sigma, i);
}

/**
 * \brief    Get theta value at position i
 * \details  --
 * \param    size_t i
 * \return   \e double
 */
inline double Individual::get_theta( size_t i ) const
{
  assert(i < _n*(_n-1)/2);
  return gsl_vector_get(_theta, i);
}

/**
 * \brief    Get the euclidean distance d(mu)
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Individual::get_dg( void ) const
{
  return _dg;
}

/**
 * \brief    Get the euclidean distance d(z)
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Individual::get_dp( void ) const
{
  return _dp;
}

/**
 * \brief    Get the fitness w(mu)
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Individual::get_wg( void ) const
{
  return _wg;
}

/**
 * \brief    Get the fitness w(z)
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Individual::get_wp( void ) const
{
  return _wp;
}

/*----------------------------------------------- MAPPING PROPERTIES */

/**
 * \brief    Get the maximum eigen value of Sigma
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Individual::get_max_Sigma_eigenvalue( void ) const
{
  return _max_Sigma_eigenvalue;
}

/**
 * \brief    Get the maximum eigen value contribution
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Individual::get_max_Sigma_contribution( void ) const
{
  return _max_Sigma_contribution;
}

/**
 * \brief    Get the dot product between Sigma maximum eigen vector and optimum direction
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Individual::get_max_dot_product( void ) const
{
  return _max_dot_product;
}

/*----------------------------------------------- MUTATIONS */

/**
 * \brief    Get the euclidean size of Mu mutation
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Individual::get_r_mu( void ) const
{
  return _r_mu;
}

/**
 * \brief    Get the euclidean size of Sigma mutation
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Individual::get_r_sigma( void ) const
{
  return _r_sigma;
}

/**
 * \brief    Get the euclidean size of Theta mutation
 * \details  --
 * \param    void
 * \return   \e double
 */
inline double Individual::get_r_theta( void ) const
{
  return _r_theta;
}

/*----------------------------
 * SETTERS
 *----------------------------*/


#endif /* defined(__SigmaFGM__Individual__) */