# xdb
  一个大的数组数据，需要时从硬盘读取，不需要时候放入硬盘，以保证内存不会超大
   
  例如一个10000000的int数组 要占内存 4*100000000/1024/1024=381M
  
  
  然后使用这个xdb
  
  #define TestNumDataSize (100000000)
  
  xdbarrfsmt<xdbarrfs<int, TestNumDataSize / 20> > a;
  
  这个a就是一个很大的数组了，然后内存只占381M/20*10(这个10是缓冲10个文件)=190M
  
		a.Init("xbd", "test", "xdb");
		for (int i = 0; i < TestNumDataSize; i++)
		{
			a.Set(i, i);
		}