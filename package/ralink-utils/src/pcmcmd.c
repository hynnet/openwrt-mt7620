#include <stdio.h>
#include <fcntl.h>
#include "pcm_ctrl.h"

pcm_record_type pcm_record;
pcm_playback_type pcm_playback;
char buffer[4096*1024];

void usage(char *cmd)
{
	printf("Usage: 0 - pcm dma start\n");
	printf("       1 - pcm dma stop\n");
	printf("       2 N (frames) I (CH) - number of frames (0~10000) for pcm record\n");
	printf("       3 N (frames) I (CH) - number of frames (0~10000) for pcm playback\n");
	printf("       4 Ch Codec (1 for ulaw, 2 for alaw, 3 for g.729ab, and 4 for g.723a)\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	FILE* fp_pcm, *fp_record_pcm, *fp_playback_pcm;
	int pcm_fd, nframe, nLen, chid;
	int total = 0, nloop;
	if (argc < 2)
		usage(argv[0]);
	
	pcm_fd = open("/dev/pcm0", O_RDWR); 
	if(pcm_fd < 0)
    	{
    		printf("open pcm driver failed (%d)...exit\n",pcm_fd);
    		return -1;
    	}	
    
	switch (atoi(argv[1])) {
	case 0:
		ioctl(pcm_fd, PCM_START, NULL);
		break;
	case 1:
		ioctl(pcm_fd, PCM_STOP, NULL);
		close(pcm_fd);
		break;
	case 2:
		if (argc < 4)
			usage(argv[0]);
		total = atoi(argv[2]);
		if((total < 0)||(total > 10000))
			usage(argv[0]);
		chid = atoi(argv[3]);
		if((chid < 0)||(chid > 2))
			usage(argv[0]);
						
		pcm_record.pcmbuf = buffer;
		if(pcm_record.pcmbuf<=0)
		{
			printf("mmap failed=%d\n",pcm_record.pcmbuf);
			return -1;
		}	
		fp_pcm = fopen("/mnt/record.pcm","wb");
	    	if(fp_pcm==NULL)
	    	{
	    		printf("open pcm file failed..exit\n");
	    		return -1;
	    	}
		ioctl(pcm_fd, PCM_SET_RECORD, chid);
		nframe = 0;
		while(nframe < total)
		{
			ioctl(pcm_fd, PCM_READ_PCM, &pcm_record);
			if(pcm_record.size>0)
			{
				nLen = fwrite(pcm_record.pcmbuf, 1, pcm_record.size, fp_pcm);
				pcm_record.size = 0;
				nframe++;
				printf("Record %d frame...\n",nframe);
			}
			//sleep(0);
		}
		ioctl(pcm_fd, PCM_SET_UNRECORD, chid);
		fclose(fp_pcm);
		close(pcm_fd);
		break;
	case 3:	
		if (argc < 4)
			usage(argv[0]);
		total = atoi(argv[2]);
		if((total < 0)||(total > 10000))
			usage(argv[0]);
		chid = atoi(argv[3]);
		if((chid < 0)||(chid > 2))
			usage(argv[0]);
						
		pcm_playback.pcmbuf = buffer;
		if(pcm_playback.pcmbuf<=0)
		{
			printf("mmap failed=%d\n",pcm_playback.pcmbuf);
			return -1;
		}	
		fp_pcm = fopen("/etc_ro/pb.pcm","rb");
	    	if(fp_pcm==NULL)
	    	{
	    		printf("open pcm file failed..exit\n");
	    		return -1;
	    	}
		ioctl(pcm_fd, PCM_SET_PLAYBACK, chid);
		nframe = 0;
		while(nframe < total)
		{
			pcm_playback.size = fread(pcm_playback.pcmbuf, 1, PCM_PAGE_SIZE, fp_pcm);
			if(pcm_playback.size>0)
			{
				ioctl(pcm_fd, PCM_WRITE_PCM, &pcm_playback);
				pcm_playback.size = 0;
				nframe++;
				printf("Playback %d frame...\n",nframe);
			}
			//sleep(0);
		}
		ioctl(pcm_fd, PCM_SET_UNPLAYBACK, chid);
		fclose(fp_pcm);
		close(pcm_fd);
		break;
	case 4:
		{
			long param[2];
			param[0] = atoi(argv[2]);
			param[1] = atoi(argv[3]);
			ioctl(pcm_fd, PCM_SET_CODEC_TYPE, param);
		}
		break;
	case 5:
		ioctl(pcm_fd, PCM_EXT_LOOPBACK_ON, NULL);
		break;
	case 6:
		ioctl(pcm_fd, PCM_EXT_LOOPBACK_OFF, NULL);
		break;
	case 7:
		nloop = 2500;
		fp_record_pcm = fopen("/mnt/rec.pcm","wb");
		fp_playback_pcm = fopen("/etc_ro/pb.pcm","rb");
		if(fp_playback_pcm==NULL)
		{
			break;	
		}
		ioctl(pcm_fd, PCM_START, 1);
		while(nloop)
		{
			fread(buffer, 1, PCM_FIFO_SIZE, fp_playback_pcm);		
			ioctl(pcm_fd, PCM_GETDATA, buffer+PCM_FIFO_SIZE);
			fwrite(buffer+PCM_FIFO_SIZE, 1, PCM_FIFO_SIZE, fp_record_pcm);		
			ioctl(pcm_fd, PCM_PUTDATA, buffer);
			nloop--;
		}	
		fclose(fp_record_pcm);
		fclose(fp_playback_pcm);
		ioctl(pcm_fd, PCM_STOP, 1);
		break;							
	default:
		{
		usage(argv[0]);
		close(pcm_fd);
		}
	}

	//close(pcm_fd);
	//printf("close pcm command\n");
	
	
    return 0;
}
