make this adjustment to compile on arm64 in functions.c:


gdouble get_cpu_speed(void)
{
    FILE *cpuinfo = fopen("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq", "r");
    if (cpuinfo == NULL)
    {
        perror("Error opening /sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq");
        return 0.0;
    }

    char line[256];
    gdouble cpu_speed_value = 0.0;

    if (fgets(line, sizeof(line), cpuinfo))
    {
        sscanf(line, "%lf", &cpu_speed_value);
    }

    fclose(cpuinfo);
    return cpu_speed_value / 1000000.0; // Convert Hz to GHz
}
