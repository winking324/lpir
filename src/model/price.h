#pragma once
#include <cinttypes>
#include <list>


namespace lpir {


struct Price {
  Price(uint32_t m, uint64_t t);

  uint32_t money;
  uint64_t ts;
};


typedef std::list<Price> PriceTrend;


}
