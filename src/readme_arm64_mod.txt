make this adjustment for cpu speed on arm architecture in functions.c:

add:

#define MAX_COMMAND_OUTPUT_SIZE 100

and change the double get_cpu_speed functon to:

double get_cpu_speed() {
    FILE *cmd_output = popen("vcgencmd measure_clock arm", "r");
    if (cmd_output == NULL) {
        return 0.0;
    }

    char output[MAX_COMMAND_OUTPUT_SIZE];
    char* frequency_string;
    double cpu_speed_value = 0.0;

    // Read command output
    if (fgets(output, sizeof(output), cmd_output)) {
        frequency_string = strstr(output, "="); // Locate the '=' character
        if (frequency_string != NULL) {
            cpu_speed_value = atof(frequency_string + 1);
        }
    }

    pclose(cmd_output);

    // Convert Hz to GHz
    return cpu_speed_value / 1000000000.0;
}


