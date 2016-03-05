/**
 * \file GainMaxColoredExpanderFilter.cpp
 */

#include "GainMaxColoredExpanderFilter.h"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace ATK
{
  template<typename DataType_>
  GainMaxColoredExpanderFilter<DataType_>::GainMaxColoredExpanderFilter(int nb_channels, size_t LUTsize, size_t LUTprecision)
  :Parent(nb_channels, LUTsize, LUTprecision), softness(static_cast<DataType_>(.0001)), max_reduction(static_cast<DataType_>(0.01)), color(0), quality(0)
  {
    recomputeLUT();
  }

  template<typename DataType_>
  GainMaxColoredExpanderFilter<DataType_>::~GainMaxColoredExpanderFilter()
  {
    //Future has to be deleted in child destructor as it uses computeGain
    if(recomputeFuture.valid())
    {
      recomputeFuture.wait();
    }
  }

  template<typename DataType_>
  void GainMaxColoredExpanderFilter<DataType_>::set_softness(DataType_ softness)
  {
    if (softness <= 0)
    {
      throw std::out_of_range("Softness factor must be strictly positive value");
    }
    this->softness = softness;
    start_recomputeLUT();
  }
  
  template<typename DataType_>
  DataType_ GainMaxColoredExpanderFilter<DataType_>::get_softness() const
  {
    return softness;
  }

  template<typename DataType_>
  void GainMaxColoredExpanderFilter<DataType_>::set_max_reduction(DataType_ max_reduction)
  {
    if (max_reduction <= 0)
    {
      throw std::out_of_range("Maximum reduction factor must be strictly positive value");
    }
    this->max_reduction = max_reduction;
    start_recomputeLUT();
  }

  template<typename DataType_>
  void GainMaxColoredExpanderFilter<DataType_>::set_max_reduction_db(DataType_ max_reduction_db)
  {
    this->max_reduction = static_cast<DataType_>(std::pow(10, max_reduction_db / 10));
    start_recomputeLUT();
  }

  template<typename DataType_>
  DataType_ GainMaxColoredExpanderFilter<DataType_>::get_max_reduction() const
  {
    return max_reduction;
  }

  template<typename DataType_>
  void GainMaxColoredExpanderFilter<DataType_>::set_color(DataType_ color)
  {
    this->color = color;
    start_recomputeLUT();
  }
  
  template<typename DataType_>
  DataType_ GainMaxColoredExpanderFilter<DataType_>::get_color() const
  {
    return color;
  }
  
  template<typename DataType_>
  void GainMaxColoredExpanderFilter<DataType_>::set_quality(DataType_ quality)
  {
    if (quality <= 0)
    {
      throw std::out_of_range("Quality factor must be a strictly positive value");
    }
    this->quality = quality;
    start_recomputeLUT();
  }
  
  template<typename DataType_>
  DataType_ GainMaxColoredExpanderFilter<DataType_>::get_quality() const
  {
    return quality;
  }

  template<typename DataType_>
  DataType_ GainMaxColoredExpanderFilter<DataType_>::computeGain( DataType_ value ) const
  {
    if(value == 0)
      return static_cast<DataType_>(std::pow(max_reduction, 1./(ratio - 1)));

    DataType diff = static_cast<DataType_>(-5 * std::log10(value * value + std::pow(max_reduction, 2. / (ratio - 1))));
    DataType additional_color = color * std::exp(- diff * diff * quality);

    return static_cast<DataType>(std::pow(10, -(std::sqrt(diff*diff + softness) + diff) / 40 * (ratio - 1))) + additional_color;
  }

  template class GainMaxColoredExpanderFilter<float>;
  template class GainMaxColoredExpanderFilter<double>;
}