#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define LINE_BUFFER 1024
#define MAX_COUNTIES 1000
#define MAX_NAME_LEN 100

// Struct to store county demographics data

//add each of the struct values to the parse data type field, number is number in small.csv for case
//then add checks for each of them in filter
typedef struct {
    char county[MAX_NAME_LEN];
    char state[MAX_NAME_LEN];
    float education_high_school_or_higher;
    float education_bachelors_or_higher;
    float ethnicity_american_indian_and_alaska_native;
    float ethnicity_asian;
    float ethnicity_black;
    float ethnicity_hispanic;
    float ethnicity_native_hawaiian_and_other_pacific_islander;
    float ethnicity_two_or_more_races;
    float ethnicity_white;
    float ethnicity_white_not_hispanic;
    int median_household_income;
    int per_capita_income;
    float persons_below_poverty_level;
    int population_2014;
} CountyData;

// Global array to store county data and count
CountyData counties[MAX_COUNTIES];
int county_count = 0;

// Function to replace underscores with spaces
void replace_underscores_with_spaces(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '_') {
            str[i] = ' ';
        }
    }
}

// Helper function to trim double quotes from a string
void trim_quotes(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[0] == '"' && str[len - 1] == '"') {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

void parse_demographics_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return;
    }

    char line[LINE_BUFFER];
    int line_number = 0;

    // Skip the header line
    if (!fgets(line, LINE_BUFFER, file)) {
        fprintf(stderr, "Error: File is empty or malformed\n");
        fclose(file);
        return;
    }

    // Read and process each subsequent line
    while (fgets(line, LINE_BUFFER, file)) {
        if (line_number == 0){
            line_number++;
            continue;
        }
        line_number++;
        if (county_count >= MAX_COUNTIES) {
            fprintf(stderr, "Error: Maximum county limit reached (%d counties)\n", MAX_COUNTIES);
            break;
        }

        // Tokenize the line by commas
        char *token;
        int field_index = 0;
        CountyData county = {0}; // Temporary struct to store the data

        token = strtok(line, ",");
        while (token) {
            trim_quotes(token); // Remove surrounding double quotes

            // Assign values to the struct fields based on the index
            switch (field_index) {
                case 0: // County
                    strncpy(county.county, token, MAX_NAME_LEN - 1);
                    break;
                case 1: // State
                    strncpy(county.state, token, MAX_NAME_LEN - 1);
                    break;
                case 5: // Education.Bachelor's Degree or Higher
                    county.education_bachelors_or_higher = atof(token);
                    break;
                case 6: // Education.High School or Higher
                    county.education_high_school_or_higher = atof(token);
                    break;
                case 11: // Ethnicities.American Indian and Alaska Native Alone
                    county.ethnicity_american_indian_and_alaska_native = atof(token);
                    break;
                case 12: // Ethnicities.Asian alone
                    county.ethnicity_asian = atof(token);
                    break;
                case 13: // Ethnicities.Black Alone
                    county.ethnicity_black = atof(token);
                    break;
                case 14: // Ethnicities.Hispanic or Latino
                    county.ethnicity_hispanic = atof(token);
                    break;
                case 15: // Ethnicities.Native Hawaiian and Other pacific Islander Alone
                    county.ethnicity_native_hawaiian_and_other_pacific_islander = atof(token);
                    break;
                case 16: // Ethnicities.two or more races
                    county.ethnicity_two_or_more_races = atof(token);
                    break;
                case 17: // Ethnicities.White alone
                    county.ethnicity_white = atof(token);
                    break;
                case 18: // Ethnicities.White alone not hispanic or latino
                    county.ethnicity_white_not_hispanic = atof(token);
                    break;
                case 25: // Income.Median Household Income
                    county.median_household_income = atoi(token);
                    break;
                case 26: // Income.Per Capita Income
                    county.per_capita_income = atoi(token);
                    break;
                case 27: // Income.Persons Below Poverty Level
                    county.persons_below_poverty_level = atof(token);
                    break;
                case 38: // Population.2014 Population
                    county.population_2014 = atoi(token);
                    break;
                default:
                    break;
            }

            // Get the next token
            token = strtok(NULL, ",");
            field_index++;
        }

        // Add the county to the array
        counties[county_count++] = county;
    }

    fclose(file);
    printf("%d entries loaded successfully.\n", county_count);
}


// Function to display all counties
void display_counties() {
    printf("Displaying County Data:\n");
    printf("----------------------------------------------------------\n");
    for (int i = 0; i < county_count; i++) {
        CountyData *c = &counties[i];
        printf("County: %s, State: %s\n", c->county, c->state);
        printf("  Education (High School or Higher): %.2f%%\n", c->education_high_school_or_higher);
        printf("  Education (Bachelors or Higher): %.2f%%\n", c->education_bachelors_or_higher);
        printf("  Ethnicities:\n");
        printf("    White: %.2f%%, Black: %.2f%%, Asian: %.2f%%, Hispanic: %.2f%%, Native Hawaiian:%.2f%%, White Alone:%.2f%%, American Indian:%.2f%%, Two or More Races:%.2f%%\n",
               c->ethnicity_white, c->ethnicity_black, c->ethnicity_asian, c->ethnicity_hispanic, c->ethnicity_native_hawaiian_and_other_pacific_islander, c->ethnicity_white_not_hispanic,c->ethnicity_american_indian_and_alaska_native,c->ethnicity_two_or_more_races);
        printf("  Income:\n");
        printf("    Median Household: $%d, Per Capita: $%d, Below Poverty: %.2f%%\n",
               c->median_household_income, c->per_capita_income, c->persons_below_poverty_level);
        printf("  Population (2014): %d\n", c->population_2014);
        printf("----------------------------------------------------------\n");
    }
}


// Function to filter counties by state abbreviation
void filter_state(const char *state_abbr) {
    int filtered_count = 0;

    // Filter entries
    for (int i = 0; i < county_count; i++) {
        if (strcmp(counties[i].state, state_abbr) == 0) {
            counties[filtered_count++] = counties[i];
        }
    }

    // Update the global county count
    county_count = filtered_count;

    // Print the result
    printf("Filter: state == %s (%d entries)\n", state_abbr, county_count);
}

void filter_field(const char *field, const char *comparison, float value) {
    int filtered_count = 0;

    // Field validation: Check if the field is supported
    int valid_field = 0;

    // List of supported fields
    const char *valid_fields[] = {
        "County",
        "State",
        "Education_High_School_or_Higher",
        "Education_Bachelors_Degree_or_Higher",
        "Ethnicity_American_Indian_and_Alaska_Native_Alone",
        "Ethnicities_Asian_Alone",
        "Ethnicities_Black_Alone",
        "Ethnicities_Hispanic_or_Latino",
        "Ethnicities_Native_Hawaiian_and_Other_Pacific_Islander_Alone",
        "Ethnicities_Two_or_More_Races",
        "Ethnicities_White_Alone",
        "Ethnicities_White_Alone_not_Hispanic_or_Latino",
        "Income_Median_Household_Income",
        "Income_Per_Capita_Income",
        "Income_Persons_Below_Poverty_Level",
        "Population_Population_2014"
    };

    // Validate the field name
    for (int i = 0; i < sizeof(valid_fields) / sizeof(valid_fields[0]); i++) {
        if (strcmp(field, valid_fields[i]) == 0) {
            valid_field = 1;
            break;
        }
    }

    if (!valid_field) {
        fprintf(stderr, "Error: Unsupported field '%s'\n", field);
        return;
    }

    // Loop through counties to filter data
    for (int i = 0; i < county_count; i++) {
        int matches = 0;

        // Perform filtering based on the field
        if (strcmp(field, "Education_High_School_or_Higher") == 0) {
            if (counties[i].education_high_school_or_higher < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].education_high_school_or_higher >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].education_high_school_or_higher <= value);
        } else if (strcmp(field, "County") == 0) {
                printf("Non-numeric values in field: %s (Line %d)\n", field, i + 2);
                continue;
        } else if (strcmp(field, "State") == 0) {
                printf("Non-numeric values in field: %s (Line %d)\n", field, i + 2);
                continue;
        } else if (strcmp(field, "Education_Bachelors_Degree_or_Higher") == 0) {
            if (counties[i].education_bachelors_or_higher < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].education_bachelors_or_higher >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].education_bachelors_or_higher <= value);
        } else if (strcmp(field, "Ethnicity_American_Indian_and_Alaska_Native_Alone") == 0) {
            if (counties[i].ethnicity_american_indian_and_alaska_native < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].ethnicity_american_indian_and_alaska_native >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].ethnicity_american_indian_and_alaska_native <= value);
        } else if (strcmp(field, "Ethnicities_Asian_Alone") == 0) {
            if (counties[i].ethnicity_asian < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].ethnicity_asian >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].ethnicity_asian <= value);
        } else if (strcmp(field, "Ethnicities_Black_Alone") == 0) {
            if (counties[i].ethnicity_black < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].ethnicity_black >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].ethnicity_black <= value);
        } else if (strcmp(field, "Ethnicities_Hispanic_or_Latino") == 0) {
            if (counties[i].ethnicity_hispanic < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].ethnicity_hispanic >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].ethnicity_hispanic <= value);
        } else if (strcmp(field, "Ethnicities_Native_Hawaiian_and_Other_Pacific_Islander_Alone") == 0) {
            if (counties[i].ethnicity_native_hawaiian_and_other_pacific_islander < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].ethnicity_native_hawaiian_and_other_pacific_islander >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].ethnicity_native_hawaiian_and_other_pacific_islander <= value);
        } else if (strcmp(field, "Ethnicities_Two_or_More_Races") == 0) {
            if (counties[i].ethnicity_two_or_more_races < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].ethnicity_two_or_more_races >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].ethnicity_two_or_more_races <= value);
        } else if (strcmp(field, "Ethnicities_White_Alone") == 0) {
            if (counties[i].ethnicity_white < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].ethnicity_white >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].ethnicity_white <= value);
        } else if (strcmp(field, "Ethnicities_White_Alone_not_Hispanic_or_Latino") == 0) {
            if (counties[i].ethnicity_white_not_hispanic < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].ethnicity_white_not_hispanic >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].ethnicity_white_not_hispanic <= value);
        } else if (strcmp(field, "Income_Median_Household_Income") == 0) {
            matches = (strcmp(comparison, "ge") == 0 && counties[i].median_household_income >= (int)value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].median_household_income <= (int)value);
        } else if (strcmp(field, "Income_Per_Capita_Income") == 0) {
            matches = (strcmp(comparison, "ge") == 0 && counties[i].per_capita_income >= (int)value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].per_capita_income <= (int)value);
        } else if (strcmp(field, "Income_Persons_Below_Poverty_Level") == 0) {
            if (counties[i].persons_below_poverty_level < 0) {
                printf("Warning: Line %d contains invalid data and will be skipped.\n", i + 2);
                continue;
            }
            matches = (strcmp(comparison, "ge") == 0 && counties[i].persons_below_poverty_level >= value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].persons_below_poverty_level <= value);
        } else if (strcmp(field, "Population_Population_2014") == 0) {
            matches = (strcmp(comparison, "ge") == 0 && counties[i].population_2014 >= (int)value) ||
                      (strcmp(comparison, "le") == 0 && counties[i].population_2014 <= (int)value);
        }

        if (matches) {
            counties[filtered_count++] = counties[i];
        }
    }

    // Update the global county count
    county_count = filtered_count;

    // Print the result
    printf("Filter: %s %s %.2f (%d entries)\n", field, comparison, value, county_count);
}

long long population_total() {
    long long total_population = 0; // Use long long to handle potentially large totals.

    // Iterate over all counties
    for (int i = 0; i < county_count; i++) {
    if (counties[i].population_2014 < 0) {
        printf("Warning: Line %d contains invalid population data and will be skipped.\n", i + 2);
        continue;
    }
    total_population += counties[i].population_2014;
    //printf("Adding %d from %s. Total so far: %lld\n", counties[i].population_2014, counties[i].county, total_population);
}


    // Print the total population
    printf("2014 population: %lld\n", total_population);
    return total_population;
}

long long population_field(const char *field) {
    const char *valid_fields[] = {
        "Education_High_School_or_Higher",
        "Education_Bachelors_Degree_or_Higher",
        "Ethnicity_American_Indian_and_Alaska_Native_Alone",
        "Ethnicities_Asian_Alone",
        "Ethnicities_Black_Alone",
        "Ethnicities_Hispanic_or_Latino",
        "Ethnicities_Native_Hawaiian_and_Other_Pacific_Islander_Alone",
        "Ethnicities_Two_or_More_Races",
        "Ethnicities_White_Alone",
        "Ethnicities_White_Alone_not_Hispanic_or_Latino",
        "Income_Persons_Below_Poverty_Level"
    };
    const int valid_field_count = sizeof(valid_fields) / sizeof(valid_fields[0]);

    // Check if the specified field is valid (case-sensitive)
    const char *matching_field = NULL;
    for (int i = 0; i < valid_field_count; i++) {
        if (strcmp(field, valid_fields[i]) == 0) {
            matching_field = valid_fields[i];
            break;
        }
    }

    // Exit if the field is invalid
    if (!matching_field) {
        fprintf(stderr, "Error: Invalid field '%s'.\n", field);
        exit(1);
    }

    long long total_sub_population = 0;

    // Process each county entry
    for (int i = 0; i < county_count; i++) {
        if (counties[i].population_2014 < 0) {
            printf("Warning: Line %d contains invalid population data and will be skipped.\n", i + 2);
            continue;
        }

        float percentage;
        if (strcmp(matching_field, "Education_Bachelor's_Degree_or_Higher") == 0) {
            percentage = counties[i].education_bachelors_or_higher;
        } else if (strcmp(matching_field, "Education_High_School_or_Higher") == 0) {
            percentage = counties[i].education_high_school_or_higher;
        } else if (strcmp(matching_field, "Ethnicity_American_Indian_and_Alaska_Native_Alone") == 0) {
            percentage = counties[i].ethnicity_american_indian_and_alaska_native;
        } else if (strcmp(matching_field, "Ethnicities_Asian_Alone") == 0) {
            percentage = counties[i].ethnicity_asian;
        } else if (strcmp(matching_field, "Ethnicities_Black_Alone") == 0) {
            percentage = counties[i].ethnicity_black;
        } else if (strcmp(matching_field, "Ethnicities_Hispanic_or_Latino") == 0) {
            percentage = counties[i].ethnicity_hispanic;
        } else if (strcmp(matching_field, "Ethnicities_Native_Hawaiian_and_Other_Pacific_Islander_Alone") == 0) {
            percentage = counties[i].ethnicity_native_hawaiian_and_other_pacific_islander;
        } else if (strcmp(matching_field, "Ethnicities_Two_or_More_Races") == 0) {
            percentage = counties[i].ethnicity_two_or_more_races;
        } else if (strcmp(matching_field, "Ethnicities_White_Alone") == 0) {
            percentage = counties[i].ethnicity_white;
        } else if (strcmp(matching_field, "Ethnicities_White_Alone_not_Hispanic_or_Latino") == 0) {
            percentage = counties[i].ethnicity_white_not_hispanic;
        } else if (strcmp(matching_field, "Income_Persons_Below_Poverty_Level") == 0) {
            percentage = counties[i].persons_below_poverty_level;
        }

        if (percentage < 0 || percentage > 100) {
            printf("Warning: Line %d contains invalid percentage data for '%s' and will be skipped.\n", i + 2, matching_field);
            continue;
        }

        // Accumulate sub-population (percentage is divided by 100 to convert to a fraction)
        total_sub_population += (long long)((percentage / 100) * counties[i].population_2014);
    }

    // Print the total sub-population
    printf("2014 %s population: %lld\n", matching_field, total_sub_population);
    return total_sub_population;
}


void percent_sub_population(const char *field){
    long long total_population = population_total();
    long long total_sub_population = population_field(field);
    double percentage = ((double)total_sub_population / total_population) * 100;
    printf("2014 %s percentage: %.2f%%\n", field, percentage);
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Invalid argument count\nUsage: %s <data_file> <operation1> [<operation2> ...]\n", argv[0]);
        return 1;
    }

    const char *data_file = argv[1];

    // Parse the demographics file
    parse_demographics_file(data_file);

    // Loop through all operations
    for (int i = 2; i < argc; i++) {
        const char *operation = argv[i];

        if (strcmp(operation, "display") == 0) {
            display_counties();
        } 
        else if (strncmp(operation, "filter-state:", 13) == 0) {
            const char *state_abbr = operation + 13;
            filter_state(state_abbr);
        }
        else if (strncmp(operation, "filter:", 7) == 0) {
            char field[MAX_NAME_LEN];
            char ge_le[3];
            float value;
            
            // Adjust parsing to handle underscores and validate the operation format
            if (sscanf(operation + 7, "%99[^:]:%2s:%f", field, ge_le, &value) == 3) {
                filter_field(field, ge_le, value);
            } else {
                fprintf(stderr, "Invalid filter operation format. Use: filter:<field>:<ge|le>:<value>\n");
                return 1;
            }
        }
        else if (strcmp(operation, "population-total") == 0) {
            population_total();
        }
        else if (strncmp(operation, "population:", 11) == 0) {
            const char *field = operation + 11;
            population_field(field);
        }
        else if (strncmp(operation, "percent:", 8) == 0) {
            const char *field = operation + 8;
            percent_sub_population(field);
        }
        else {
            fprintf(stderr, "Unknown operation: %s\n", operation);
            return 1;
        }
    }

    return 0;
}