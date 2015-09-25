package com.elex.chatservice;

import java.util.Comparator;
import java.util.HashMap;

class RankComparator
  implements Comparator<String>
{
  HashMap<String, Double> base;

  public RankComparator(HashMap<String, Double> base)
  {
    this.base = base;
  }

  public int compare(String d1, String d2)
  {
    Double r1 = (Double)this.base.get(d1);
    Double r2 = (Double)this.base.get(d2);
    if (r1.doubleValue() >= r2.doubleValue()) {
      return -1;
    }
    return 1;
  }
}