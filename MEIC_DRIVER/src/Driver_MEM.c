#include "Driver_MEM.h"


#define MEM_PORT_SUM  32*1024   
#define MEM_BLOCK_SIZE      32        
#define MEM_LIST       MEM_PORT_SUM/MEM_BLOCK_SIZE
#define  Numofblock(size)  (size%MEM_BLOCK? size/MEM_BLOCK+1: size/MEM_BLOCK)

const u32 MEM_LMAP=MEM_LIST;
const u32 MEM_PORT_FULL=MEM_PORT_SUM;
const u32 MEM_BLOCK = MEM_BLOCK_SIZE;

__align(4) u8 MEM_PORT[MEM_PORT_SUM]; 
u16  MEM_STATEMAP[MEM_LIST];


static void  MEM_Init(void);

static void MEM_Set(void* addr,u16 value,u16 length);

/*------------
内存管理控制器
*/

struct MEM_Control
{
	void (*init)(void);  //初始化
  u8  (*UsageofMEM)(void);//内存剩余量
	u8  (*memport);
	u16  (*memmap);
	u8   MEM_STATE;
};
struct  MEM_Control  MEM_Driver=
{
	MEM_Init,
	MEM_Used,
  MEM_PORT,	
	MEM_STATEMAP,
	0,
};
u8  MEM_Used(void)
{
	u32 used=0;
	u16 i=0;
	while(i<MEM_LMAP)
	{
		if(MEM_Driver.memmap[i])
		{
			used++;
		}
		i++;
	}	
	return (used*100)/MEM_LMAP;
}
/*内存设置值函数*/
void MEM_Set(void* addr,u16 value,u16 length)
{
	u8 *p;
	p = addr;
	while(length--)
	{
		*p++=value;
	}
}
/* 内存管理初始化*/
void  MEM_Init(void)
{
	MEM_Set(MEM_Driver.memport,0,MEM_PORT_FULL);
	MEM_Set(MEM_Driver.memmap,0,MEM_LMAP*2);
	MEM_Driver.MEM_STATE = 1;
}

/*内存空间申请*/
void* MEM_malloc(u16 msize)
{
			u32  offset=0,num=0,temp=0;
			u32 block;		
	    if(!MEM_Driver.MEM_STATE)
		  {
			   MEM_Driver.init();
		  }
			if(msize==0) return NULL;
      block = Numofblock(msize);										
			while(offset<MEM_LMAP)
	 		{			
				if(!MEM_Driver.memmap[offset])  num++;
				else   num =0 ;
				if(num == block)
				{  		            
					temp = offset-num+1;
					while(temp<offset+1)
					{
						  MEM_Driver.memmap[temp] = msize;
   				    temp++;						  					  
					}
						
					temp = offset-num+1;
					return (void*)(temp*MEM_BLOCK+(u32)MEM_Driver.memport);
				}
				
				offset++;
			}		
			return NULL;
}
/*内存空间释放*/
u8  MEM_free(u8 *p)
{
	  u8 temp,block;
	  if(!MEM_Driver.MEM_STATE)
		{
			return 0;
		}
  	temp =(p-MEM_Driver.memport)/MEM_BLOCK;   	
  	block =Numofblock(MEM_Driver.memmap[temp]);   	  	
    MEM_Set(p,0, MEM_Driver.memmap[temp]);
    MEM_Set(&MEM_Driver.memmap[temp],0, block*2);  
    return 1;		
}
/*-------------------------兼容FATFS 底层函数-----------------------------*/


//u32 mem_malloc(u32 size)  
//{  
//    signed long offset=0;  
//    u16 nmemb;	//需要的内存块数  
//	  u16 cmemb=0;//连续空内存块数
//    u32 i;  
//       if(!MEM_Driver.MEM_STATE)
//		  {
//			   MEM_Driver.init();
//		  } 	//未初始化,先执行初始化 
//    if(size==0)return 0XFFFFFFFF;				//不需要分配
//    nmemb=size/MEM_BLOCK;  					//获取需要分配的连续内存块数
//    if(size%MEM_BLOCK)nmemb++;  
//    for(offset=MEM_LMAP-1;offset>=0;offset--)	//搜索整个内存控制区  
//    {     
//		if(!MEM_Driver.memmap[offset])cmemb++;	//连续空内存块数增加
//		else cmemb=0;							//连续内存块清零
//		if(cmemb==nmemb)						//找到了连续nmemb个空内存块
//		{
//            for(i=0;i<nmemb;i++)  				//标注内存块非空 
//            {  
//                MEM_Driver.memmap[offset+i]=nmemb;  
//            }  
//            return (offset*MEM_BLOCK);			//返回偏移地址  
//		}
//    }  
//    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
//}  


////分配内存(外部调用)
////size:内存大小(字节)
////返回值:分配到的内存首地址.
//void *mymalloc(u32 size)  
//{  
//    u32 offset;  									      
//	offset=mem_malloc(size);  	   				   
//    if(offset==0XFFFFFFFF)return NULL;  
//    else return (void*)((u32)MEM_Driver.memport+offset);  
//}  
//u8 mem_free(u32 offset)  
//{  
//    int i;  
//       if(!MEM_Driver.MEM_STATE)
//		  {
//			   MEM_Driver.init();
//		  } 
//    if(offset<MEM_PORT_FULL)//偏移在内存池内. 
//    {  
//        int index=offset/MEM_BLOCK;		//偏移所在内存块号码  
//        int nmemb=MEM_Driver.memmap[index];	//内存块数量
//        for(i=0;i<nmemb;i++)  				//内存块清零
//        {  
//            MEM_Driver.memmap[index+i]=0;  
//        }
//        return 0;  
//    }else return 2;//偏移超区了.  
//}  
////释放内存(外部调用) 
////ptr:内存首地址 
//void myfree(void *ptr)  
//{  
//	u32 offset;  
//    if(ptr==NULL)return;//地址为0.  
// 	offset=(u32)ptr-(u32)MEM_Driver.memport;  
//    mem_free(offset);	//释放内存     
//}  

