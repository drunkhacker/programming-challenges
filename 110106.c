#include <stdio.h>
#include <string.h>

int registers[10];
int ram[1000];

#define OP(inst) ((inst)/100)
#define OP1(inst) (((inst)/10)%10)
#define OP2(inst) ((inst)%10)

int process()
{
    int pc = 0;
    int inst;
    int exe_inst = 0;

    /*for (pc = 0; pc < 1000; pc++) {*/
        /*printf("%d\n", ram[pc]);*/
    /*}*/

    /* 
     * 100 means halt
     * 2dn means set register d to n (between 0 and 9)
     * 3dn means add n to register d
     * 4dn means multiply register d by n
     * 5ds means set register d to the value of register s
     * 6ds means add the value of register s to register d
     * 7ds means multiply register d by the value of register s
     * 8da means set register d to the value in RAM whose address is in register a
     * 9sa means set the value in RAM whose address is in register a to the value of register s
     * 0ds means goto the location in register d unless register s contains 0 
     */
    while ((inst = ram[pc++]) != 100) {
        /*printf("inst:%03d, OP:%d, OP1:%d, OP2:%d\n", inst, OP(inst), OP1(inst), OP2(inst));*/
        switch (OP(inst)) {
            case 2:
                registers[OP1(inst)] = OP2(inst);
                registers[OP1(inst)] %= 1000;
                break;
            case 3:
                registers[OP1(inst)] += OP2(inst);
                registers[OP1(inst)] %= 1000;
                break;
            case 4:
                registers[OP1(inst)] *= OP2(inst);
                registers[OP1(inst)] %= 1000;
                break;
            case 5:
                registers[OP1(inst)] = registers[OP2(inst)];
                registers[OP1(inst)] %= 1000;
                break;
            case 6:
                registers[OP1(inst)] += registers[OP2(inst)];
                registers[OP1(inst)] %= 1000;
                break;
            case 7:
                registers[OP1(inst)] *= registers[OP2(inst)];
                registers[OP1(inst)] %= 1000;
                break;
            case 8:
                registers[OP1(inst)] = ram[registers[OP2(inst)]];
                registers[OP1(inst)] %= 1000;
                break;
            case 9:
                ram[registers[OP2(inst)]] = registers[OP1(inst)];
                ram[registers[OP2(inst)]] %= 1000;
                break;
            case 0:
                if (registers[OP2(inst)] != 0)
                    pc = registers[OP1(inst)];
                break;
        }
        exe_inst++;
    }
    exe_inst++; /* including halt command */

    return exe_inst;
}

int main()
{
    int cases;
    int i = -1;
    char buf[BUFSIZ];
    int inst;
    int *ramp;
    int exe_cnt;

    scanf("%d", &cases);
    fgets(buf, BUFSIZ-1, stdin);
    while(fgets(buf, BUFSIZ-1, stdin)) {
        if (buf[0] == '\n')  {
            if (++i > 0) 
                printf("%d\n", process());

            memset(registers, 0, 10);
            memset(ram, 0, 1000);
            exe_cnt = 0;
            ramp = &ram[0];

            continue;
        }

        sscanf(buf, "%d", &inst);
        *ramp++ = inst;
    }
    printf("%d\n", process());
    return 0;
}

/* vim: set ts=4 sw=4 : */
