/*
	Copyright (C) 2016 Alexandros Tsonis, Sotiris Papatheodorou

	This file is part of NeuroticNetwork.

	NeuroticNetwork is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	NeuroticNetwork is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with NeuroticNetwork.  If not, see <http://www.gnu.org/licenses/>.
*/

//Neural Network Testing

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "neuron.h"
#include "network.h"
#ifndef _DEBUG
#define _DEBUG 0
#endif
int main() {

	nn_info();

    int npl[5],ret=0,retbef=0,count,tempret,neuroncounter,i;
    double *input,*intendedout;
		char filename[100],originalpath[90];
		FILE** NW;
    struct neural_net *net;
		char valid=0,inp;
		bool IS_LOGGING;
    npl[0] = 6;
    npl[1] = 5;
    npl[2] = 2;
    //npl[3] = 5;
    //npl[4] = 2;
    ret = create_network(3, 3, npl, &net);//num of inputs, num of layers
    if (ret == _CREATION_MEMORY_ERROR) {
        return EXIT_FAILURE;
    }
		while (valid==0){
			printf("\n Do you want to log the weights of the neurons after each learning iteration? (Y/n)");
			inp=fgetc(stdin);
			if (inp=='y' || inp=='Y')
			{
				valid = 1;
				IS_LOGGING=true;
				fflush(stdin);
			}
			else if (inp=='n' || inp=='N')
			{
				valid = 1;
				IS_LOGGING=false;
				fflush(stdin);
			}
			else
			{
				printf("\nWrong input! Please enter either Y or n\n");
				fflush(stdin);
			}
		}
		if (IS_LOGGING==true){
			startLogging(net);
		}
    printf("Function returned %d\n Press any key", ret);
    scanf("%d", &ret);
    ret = EXIT_FAILURE;
    count = 0;
    input = malloc(8 * 3 * sizeof(double));
    intendedout = malloc(8 * 2 * sizeof(double));
    input[0] = 0;
    input[1] = 0;
    input[2] = 0;
    intendedout[0] = 0;
    intendedout[1] = 1;
    printf("%lf\t%lf\t%lf\n", input[count * 3], input[count * 3 + 1], input[count * 3 + 2]);
    for (count = 1;count < 8;count++) {
        input[count*3] = count % 2;
        input[count * 3 + 1] = input[count * 3 - 2];
        input[count * 3 + 2] = input[count * 3 - 1];
        if (input[count * 3] == 0) {
            input[count * 3 + 1] = ((int)input[count * 3 - 2] + 1) % 2;
            if (input[count * 3 + 1] == 0) {
                input[count * 3 + 2] = ((int)input[count * 3 - 1] + 1) % 2;
            }
        }
        printf("%lf\t%lf\t%lf\n", input[count * 3], input[count * 3 + 1], input[count * 3 + 2]);
        intendedout[count * 2] = ((int)input[count*3]+(int)input[count*3+1]+(int)input[count*3+2]) % 2;
        intendedout[count * 2 + 1] = ((int)intendedout[count*2] + 1) % 2;
        printf("%lf\t%lf\n", intendedout[count * 2], intendedout[count * 2 + 1]);
    }
    network_print(net);
    printf("Beginning learning process\n");
    network_print(net);
    count = 0;

		if (IS_LOGGING==true){

		}
    while (network_test(net, input, intendedout, 8)) {
        network_learn(net, input, intendedout, 8);

				//Log neuron weights in files
				if (IS_LOGGING==true){
					networkLogging(net);
				}
        count++;
        if (count % 10000 == 0) {
            count = 0;
            //network_print_whole_out(net);
            printf("%d", network_test(net, input, intendedout, 8));
        }
    }
		//Close file streams (weights don't get modified after this)
		if (IS_LOGGING==true){
			stopLogging(net);
		}
    printf("Function returned %d\n Press any key", ret);
    printf("Enter first input\n 380 is the exit number\n");
    scanf("%lf", &input[0]);
    while (input[0] != 380) {
        printf("\nEnter second input\n");
        scanf("%lf", &input[1]);
        printf("\nEnter third input\n");
        scanf("%lf", &input[2]);
        change_input(input, net);
        network_activate(net);
        //network_print_whole_out(net);
        network_print_output_only(net);
        printf("Enter first input\n 380 is the exit number\n");
        scanf("%lf", input);
    }
    network_delete(net);



    return EXIT_SUCCESS;
}
