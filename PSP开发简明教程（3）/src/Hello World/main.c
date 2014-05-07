#include <pspkernel.h>
#include <pspdebug.h>

//������ص���Ϣ,�����ǳ������ơ�����ģʽ�����汾�ţ��ΰ汾��
PSP_MODULE_INFO("Hello World", PSP_MODULE_USER, 1, 0);

//�������̵߳�����
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 
PSP_HEAP_SIZE_MAX();

//Ϊ��ʹ�÷��㣬��pspDebugScreenPrintf����Ϊprintf���÷�Ҳ��printfһ����
#define printf	pspDebugScreenPrintf

int done = 0;

///////////////////////////////////////////////////////////////

/* Exit callback */
int exit_callback(int arg1, int arg2, void *common)
{
	done = 1;
	return 0;
}

/* Callback thread */
int CallbackThread(SceSize args, void *argp)
{
	int cbid;

	cbid = sceKernelCreateCallback("Exit Callback", exit_callback, NULL);
	sceKernelRegisterExitCallback(cbid);
	sceKernelSleepThreadCB();
	return 0;
}

/* Sets up the callback thread and returns its thread id */
int SetupCallbacks(void)
{
	int thid = 0;

	thid = sceKernelCreateThread("update_thread", CallbackThread,
					 0x11, 0xFA0, 0, 0);
	if(thid >= 0)
	{
		sceKernelStartThread(thid, 0, 0);
	}

	return thid;
}

///////////////////////////////////////////////////////////////

int main(void)
{
	//��ʼ��DebugScreen�������Ϳ�������Ļ����ʾ��
	pspDebugScreenInit();
	
	//�����̼߳����û���Ϊ��Ϊ���ܹ��˳�����
	SetupCallbacks();

	//��ѭ���壬�û��˳�ʱ��done���Ϊ1��ѭ���˳�
	while(!done)
	{
		pspDebugScreenSetXY(0, 0);
		printf("Hello World!\n");
		printf("Test by Max");
		sceDisplayWaitVblankStart();
	}
	
	//�����˳�
	sceKernelExitGame();	
	
	return 0;
}