
using System;

/*
 * Create some objects of class FourLongObject. This test is a part of Java
 * Grande Forum Benchmark suite. It is taken from JGFCreateBench benchmark.
 */  
namespace CSGrande {

  class BasicFourLongObject : IBench {

    public const int INITSIZE = 10000;
    public const int MAXSIZE = 100000;
    public const int OBJECTS_COUNT = 16;

    public void RunBench() {

      FourLongObject obj;

      for(int size = INITSIZE; size < MAXSIZE; size *= 2)
        for(int i = 0; i < size; i++)
	  for(int j = 0; j < OBJECTS_COUNT; j++)
	    obj = new FourLongObject();
      System.GC.Collect();

    }

  }

}
