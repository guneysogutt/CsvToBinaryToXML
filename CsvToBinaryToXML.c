#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
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
#define MAX_CURRENCY_UNIT_LEN 4
#define MAX_AVAILABLE_LOAN_LEN 5



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
    char currency_unit[MAX_CURRENCY_UNIT_LEN];// "€","₺","$"
    int totalBalanceAvailable;// stores the available balance of the customer
    char availableForLoan[MAX_AVAILABLE_LOAN_LEN]; // it can be ":)" or ":("
} ;

typedef struct _customer customer; // use the _customer as customer

customer customers[51];

// this function adds a space between two consecutive commas
void addCommaSpace(char *str) { 
    char *p = str;
    
    int size = strlen(str); // get the size of the line
    
    int index = 0; // current index
    
    while (*p) {

        if (*p == ',' && *(p+1) == ',') { // if a consecutive commas occurs
            
            *p = ',';
            for(int i = size; i > index + 2; i--){ // move each char one cell right in order to allocate the space without losing data
                str[i] = str[i-1];
            }
            *(p+1) = ' '; // add a space between them
            *(p+2) = ','; // put the original comma back
            
        } else { // if no consecutive commas
            p++; // just increase the pointer
        }
        index++; // increase the index
    }
} // end of addCommaSpace


void csv_to_bin (char* input_file, char* output_file){

    FILE *fp; // create file pointer


    int num_customers = 0; // set the initial customer number before adding to the struct array

    fp = fopen(input_file, "r"); //open a stream with read mode


    if(fp == NULL){ // if an error occurs opening a file
        printf("Error. Can't open csv file!\n");
        exit (1);
    }


    char line[sizeof (customer)]; // create the line to be read variable

    int line_length = sizeof (customer); // get the size of the structure in bytes


    fgets(line, line_length, fp); // skip the header line


    // read each subsequent line using sscanf()
    while (fgets(line, line_length, fp) != NULL) {

        addCommaSpace(line);
        // this implementation found from IBM documentation
        //%[^,] means that read until the comma, but don't include it
        // "%[^,],%[^,],%c,%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%c\n"
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%s\n",
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


    fp = fopen(output_file, "w"); //open a stream with write mode

    if(fp == NULL) // if an error occurs opening a file
    {
        printf("Error. Can't open dat file!\n");
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
        printf("Currency Unit: %s\n", customers[i].currency_unit);
        printf("Total Balance Available: %d\n", customers[i].totalBalanceAvailable);
        printf("Available for Loan: %s\n", customers[i].availableForLoan);
        printf("\n");

    }

} // end csv_to_bin

void binary_to_XML(char* input_file,char* output_file){
    // CONVERTIRNG BINARY TO XML

    FILE* fp;

    xmlDocPtr records_document = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, row_node = NULL, customer_info_node = NULL, bank_account_info_node = NULL, total_balance_available_node;  /* node pointers */
    char buff[256];
    
    
    records_document = xmlNewDoc(BAD_CAST "1.0");       // Creating the XML file
    root_node = xmlNewNode(NULL, BAD_CAST input_file);   // Creating the root node of records XML file
    xmlDocSetRootElement(records_document, root_node);  // Assigning the previous root node as root node of the document

    customer readItem;
    
    fp = fopen(input_file, "r"); // Creating file pointer to read binary file

    if(fp == NULL)  // Checking if the file created correctly
    {
        printf("Error. Can't open binary file.");
        exit(1);
    }

    for (size_t i = 1; i < sizeof(customers)/sizeof(customer); i++) // Looping through file
    {
        row_node = xmlNewChild(root_node, NULL, BAD_CAST "row", NULL);  // Creating id node and assigning it as child of root node
        sprintf(buff, "%d", i);
        xmlNewProp(row_node, BAD_CAST "id", BAD_CAST buff);    // Adding id attribute to row node

        customer_info_node = xmlNewChild(row_node, NULL, BAD_CAST "customer_info", NULL);   // Creating customer info node and assigning it as child of root node
        bank_account_info_node = xmlNewChild(row_node, NULL, BAD_CAST "bank_account_info", NULL);   // Creating bank account info node and assigning it as child of root node

        fread(&readItem, sizeof(customer), 1, fp); // Reading current customer data

        xmlNewChild(customer_info_node, NULL, BAD_CAST "name", readItem.name); // Adding name node as child of customer_info node
        xmlNewChild(customer_info_node, NULL, BAD_CAST "surname", readItem.surname); // Adding surname node as child of customer_info node
        sprintf(buff, "%c", readItem.gender);
        xmlNewChild(customer_info_node, NULL, BAD_CAST "gender", buff); // Adding gender node as child of customer_info node
        xmlNewChild(customer_info_node, NULL, BAD_CAST "occupancy", readItem.occupancy); // Adding occupancy node as child of customer_info node
        xmlNewChild(customer_info_node, NULL, BAD_CAST "level_of_education", readItem.levelOfEducation); // Adding level_of_education node as child of customer_info node
        xmlNewChild(customer_info_node, NULL, BAD_CAST "email", readItem.email); // Adding email node as child of customer_info node
        
        xmlNewChild(bank_account_info_node, NULL, BAD_CAST "bank_account_number", readItem.bankAccountNumber); // Adding bank_account_number node as child of bank_account_info node
        xmlNewChild(bank_account_info_node, NULL, BAD_CAST "IBAN", readItem.IBAN); // Adding IBAN node as child of bank_account_info node
        xmlNewChild(bank_account_info_node, NULL, BAD_CAST "account_type", readItem.accountType); // Adding account_type node as child of bank_account_info node
        
        sprintf(buff, "%d", readItem.totalBalanceAvailable);
        total_balance_available_node = xmlNewChild(bank_account_info_node, NULL, BAD_CAST "total_balance_available", buff); // Adding total_balance_available node as child of bank_account_info node
        
        sprintf(buff, "%s", readItem.currency_unit);
        xmlNewProp(total_balance_available_node, BAD_CAST "currency_unit", BAD_CAST buff);  // Adding currency_unit attribute to bank_account_info node
        xmlNewProp(total_balance_available_node, BAD_CAST "bigEnd_Version", BAD_CAST "Will be updated");  // Adding bigEnd_Version attribute to bank_account_info node

        sprintf(buff, "%s", readItem.availableForLoan);
        xmlNewChild(bank_account_info_node, NULL, BAD_CAST "available_for_loan", buff); // Adding available_for_loan node as child of bank_account_info node
    } 


    

    fclose(fp);
    /*
     * Dumping document to stdio or file
     */
    xmlSaveFormatFileEnc(output_file, records_document, "UTF-8", 1);
    xmlSaveFormatFileEnc("-", records_document, "UTF-8", 1);
	
	
    //xmlSaveFormatFile
    
    /*free the document */
    xmlFreeDoc(records_document );
    /*
     *Free the global variables that may
     *have been allocated by the parser.
     */
    xmlCleanupParser();
    //xmlMemoryDump();

} // end binary_to_XML

void XSD_validation(char* input_file, char* output_file){
    printf("This function will be updated\n");
}

int main(int argc, char **argv) {


    // set and assign the command query file names and type
    char* input_file = argv[1];
    char* output_file = argv[2];
    int type = atoi(argv[3]);


    // Assign command query types
    switch(type){ 
        case 1: 
            csv_to_bin(input_file,output_file);
            break;
        case 2: 
            binary_to_XML(input_file,output_file);
            break;
        case 3:
            XSD_validation(input_file,output_file);
            break;    
        default:
            printf("Invalid query!\n");
            break;    
    }
    

    return 0;
}