# xdb
  һ������������ݣ���Ҫʱ��Ӳ�̶�ȡ������Ҫʱ�����Ӳ�̣��Ա�֤�ڴ治�ᳬ��
   
  ����һ��10000000��int���� Ҫռ�ڴ� 4*100000000/1024/1024=381M
  
  
  Ȼ��ʹ�����xdb
  
  #define TestNumDataSize (100000000)
  
  xdbarrfsmt<xdbarrfs<int, TestNumDataSize / 20> > a;
  
  ���a����һ���ܴ�������ˣ�Ȼ���ڴ�ֻռ381M/20*10(���10�ǻ���10���ļ�)=190M
  
		a.Init("xbd", "test", "xdb");
		for (int i = 0; i < TestNumDataSize; i++)
		{
			a.Set(i, i);
		}