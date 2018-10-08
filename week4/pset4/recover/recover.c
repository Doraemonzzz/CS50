#include <stdio.h>
#include <cs50.h>

const int N = 512;

int main(int argc, char *argv[]){
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr, "Usage: copy infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[1];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    //buffer
    unsigned char buffer[N];
    //记录文件数量
    int cnt = 0;
    //判断是否找到
    bool flag = false;
    //文件名
    char filename[100];
    //文件指针
    FILE *img = malloc(sizeof(FILE));

    while(fread(&buffer, N, 1, inptr)){
        //判别条件，表示开始
        if (buffer[0] == 0xff &&
            buffer[1] == 0xd8 &&
            buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0){
                if(flag == false){
                    flag = true;
                }else{
                    //正在就关闭
                    fclose(img);
                }
                //保存文件
                sprintf(filename, "%03d.jpg", cnt);
                cnt++;
                img = fopen(filename, "w");
                fwrite(&buffer, 512, 1, img);
            }else{
                //打开文件后才开始写入
                if(flag){
                    fwrite(&buffer, 512, 1, img);
                }
            }
    }

    // close infile
    fclose(inptr);
    fclose(img);

    return 0;
}