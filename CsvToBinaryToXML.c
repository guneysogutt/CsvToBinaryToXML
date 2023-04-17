#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma pack(1)


// set the lengths of the strings
#define MAX_NAME_LEN 21
#define MAX_SURNAME_LEN 31
#define MAX_OCCUPANCY_LEN 31
#define MAX_EDUCATION_LEN 4
#define MAX_EMAIL_LEN 31
#define MAX_BANK_ACC_LEN 13
#define MAX_IBAN_LEN 28
#define MAX_ACC_TYPE_LEN 14


// the structure of customer
typedef struct _customer {
    char name[MAX_NAME_LEN];// name is assumed at most 20 characters
    char surname[MAX_SURNAME_LEN];// surname is assumed at most 30 characters
    char gender;// M(male) or F(female)
    char occupancy[MAX_OCCUPANCY_LEN];// occupancy is assumed at most 30 characters
    char levelOfEducation[MAX_EDUCATION_LEN];// it can be at most 3 chars eg: "PhD","MSc","PS", etc.
    char email[MAX_EMAIL_LEN];// ends with @google,@yahoo or @hotmail.com
    char bankAccountNumber[MAX_BANK_ACC_LEN];// the format is 0123-4567890
    char IBAN[MAX_IBAN_LEN];// the format is : TR12 3456 7890 1234 5678 90
    char accountType[MAX_ACC_TYPE_LEN];// it can be "deposit","drawing","daily_deposit"
    char currency_unit;// "€","₺","$"
    int totalBalanceAvailable;// stores the available balance of the customer
    char availableForLoan; // it can be ":)" or ":("
} ;

typedef struct _customer customer; // use the _customer as customer

int main() {

    FILE *fp; // create file pointer

    customer customers[51];

    int num_customers = 0; // set the initial customer number before adding to the struct array

    fp = fopen("..\\records.csv", "r"); //open a stream with read mode


    if(fp == NULL){ // if an error occurs opening a file
        printf("error opening file!\n");
        exit (1);
    }


    char line[sizeof (customer)]; // create the line to be read variable

    int line_length = sizeof (customer); // get the size of the structure in bytes


    fgets(line, line_length, fp); // skip the header line


    // read each subsequent line using sscanf()
    while (fgets(line, line_length, fp) != NULL) {

        // this implementation found from IBM documentation
        //%[^,] means that read until the comma, but don't include it
        // "%[^,],%[^,],%c,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%c\n"
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%[^,]\n",
               &customers[num_customers].name,
               &customers[num_customers].surname,
               &customers[num_customers].gender,
               &customers[num_customers].occupancy,
               &customers[num_customers].levelOfEducation,
               &customers[num_customers].email,
               &customers[num_customers].bankAccountNumber,
               &customers[num_customers].IBAN,
               &customers[num_customers].accountType,
               &customers[num_customers].currency_unit,
               &customers[num_customers].totalBalanceAvailable,
               &customers[num_customers].availableForLoan
        );

        num_customers++; // increase the index number
    }



    fclose(fp); // close the file


    fp = fopen("records.dat", "wb"); //open a stream with write mode

    if(fp == NULL) // if an error occurs opening a file
    {
        printf("error opening file!\n");
        exit (1);
    }

    // write the data in the structure array into a .dat file
    fwrite(customers,sizeof (customer),num_customers,fp);

    fclose(fp); // close the file

    // Print out the customers
    for (int i = 0; i < num_customers; i++) {

        printf("Customer %d:\n", i + 1);
        printf("Name: %s %s\n", customers[i].name, customers[i].surname);
        printf("Gender: %c\n", customers[i].gender);
        printf("Occupancy: %s\n", customers[i].occupancy);
        printf("Level of Education: %s\n", customers[i].levelOfEducation);
        printf("Email: %s\n", customers[i].email);
        printf("Bank Account Number: %s\n", customers[i].bankAccountNumber);
        printf("IBAN: %s\n", customers[i].IBAN);
        printf("Account Type: %s\n", customers[i].accountType);
        printf("Currency Unit: %c\n", customers[i].currency_unit);
        printf("Total Balance Available: %d\n", customers[i].totalBalanceAvailable);
        printf("Available for Loan: %c\n", customers[i].availableForLoan);
        printf("\n");
    };

    return 0;
}