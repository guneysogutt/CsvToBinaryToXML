#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlschemastypes.h>
#pragma pack(1)


// set the lengths of the strings
#define MAX_NAME_LEN 21
#define MAX_SURNAME_LEN 31
#define MAX_OCCUPANCY_LEN 31
#define MAX_EDUCATION_LEN 4
#define MAX_EMAIL_LEN 256
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
    char level_of_education[MAX_EDUCATION_LEN];// it can be at most 3 chars eg: "PhD","MSc","PS", etc.
    char email[MAX_EMAIL_LEN];// ends with @google,@yahoo or @hotmail.com
    char bank_account_number[MAX_BANK_ACC_LEN];// the format is 0123-4567890
    char IBAN[MAX_IBAN_LEN];// the format is : TR12 3456 7890 1234 5678 90
    char account_type[MAX_ACC_TYPE_LEN];// it can be "deposit","drawing","daily_deposit"
    char currency_unit[MAX_CURRENCY_UNIT_LEN];// "€","₺","$"
    int total_balance_available;// stores the available balance of the customer
    char available_for_loan[MAX_AVAILABLE_LOAN_LEN]; // it can be ":)" or ":("
} ;

typedef struct _customer customer; // use the _customer as customer


// this function adds a space between two consecutive commas
void addCommaSpace(char *str) {
    char *p = str;

    int size = strlen(str); // get the size of the line

    int index = 0; // current index

    while (*p) {

        if (*p == ',' && *(p+1) == ',') { // if consecutive commas occurs

            *p = ',';
            for(int i = size; i > index + 2; i--){ // move each char one cell right in order to allocate the space without losing data
                str[i] = str[i-1];
            }
            *(p+1) = ' '; // add a space between them
            *(p+2) = ','; // put the original comma back

        } else { // if no consecutive commas
            p++; // just increase the pointer
        }
        index++; // increase the index for each loop
    }
} // end of addCommaSpace


// CONVERTING CSV FILE TO BINARY FILE
void csv_to_bin (char* input_file, char* output_file){

    FILE *fp; // create reading file pointer
    FILE *fp2; // create writing file pointer

    fp = fopen(input_file, "r"); //open a stream with read mode
    fp2 = fopen(output_file, "w"); //open a stream with write mode

    customer readItem;

    if(fp == NULL || fp2 == NULL){ // if an error occurs opening a file
        printf("Error. Can't open the file!\n");
        exit (1);
    }

    int line_length = sizeof (customer); // get the size of the structure in bytes

    char line[line_length]; // create the line to be read variable


    fgets(line, line_length, fp); // skip the header line

    int num_customers = 0; // counts the number of the customers

    // read each subsequent line using sscanf()
    //then assign the data for each attribute of the customer structure seperately
    while (fgets(line, line_length, fp) != NULL) {

        addCommaSpace(line); // add space if there are consecutive commas


        // this implementation found from IBM documentation
        //%[^,] means that read until the comma, but don't include it
        // "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%s\n"
        sscanf(line, "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%d,%s\n",
               &readItem.name,
               &readItem.surname,
               &readItem.gender,
               &readItem.occupancy,
               &readItem.level_of_education,
               &readItem.email,
               &readItem.bank_account_number,
               &readItem.IBAN,
               &readItem.account_type,
               &readItem.currency_unit,
               &readItem.total_balance_available,
               &readItem.available_for_loan
        );

                // Since the program adds ' ' to 2 consecutive comma and read item takes ' ' as value to corresponding variable
                // below lines will converts ' ' to null 
               if(*readItem.name == ' ') *readItem.name = NULL;
               if(*readItem.surname == ' ') *readItem.surname = NULL;
               if(readItem.gender == ' ') readItem.gender = NULL;
               if(*readItem.occupancy == ' ') *readItem.occupancy = NULL;
               if(*readItem.level_of_education == ' ') *readItem.email = NULL;
               if(*readItem.email == ' ') *readItem.email = NULL;
               if(*readItem.bank_account_number == ' ') *readItem.bank_account_number = NULL;
               if(*readItem.IBAN == ' ') *readItem.IBAN = NULL;
               if(*readItem.account_type == ' ') *readItem.account_type = NULL;
               if(*readItem.currency_unit == ' ') *readItem.currency_unit = NULL;
               if(readItem.total_balance_available == ' ') readItem.total_balance_available = NULL;
               if(*readItem.available_for_loan == ' ') *readItem.available_for_loan = NULL;

        // write the data in the structure array into a .dat file
        fwrite(&readItem,line_length,1,fp2);

        // print the customers
        printf("Customer %d:\n", num_customers + 1);
        printf("Name: %s %s\n", readItem.name, readItem.surname);
        printf("Gender: %c\n", readItem.gender);
        printf("Occupancy: %s\n", readItem.occupancy);
        printf("Level of Education: %s\n", readItem.level_of_education);
        printf("Email: %s\n", readItem.email);
        printf("Bank Account Number: %s\n", readItem.bank_account_number);
        printf("IBAN: %s\n", readItem.IBAN);
        printf("Account Type: %s\n", readItem.account_type);
        printf("Currency Unit: %s\n", readItem.currency_unit);
        printf("Total Balance Available: %d\n", readItem.total_balance_available);
        printf("Available for Loan: %s\n", readItem.available_for_loan);
        printf("\n");


        num_customers++; // increase the customer number
    }


    fclose(fp); // close the file
    fclose(fp2); // close the file

} // end csv_to_bin


// CONVERTING BINARY TO XML
void binary_to_XML(char* input_file,char* output_file){

    FILE* fp;

    xmlDocPtr records_document = NULL;       /* document pointer */
    xmlNodePtr root_node = NULL, row_node = NULL, customer_info_node = NULL, bank_account_info_node = NULL, total_balance_available_node;  /* node pointers */
    char buff[256];



    records_document = xmlNewDoc(BAD_CAST "1.0");       // Creating the XML file

    //Storing the part of the output file name before the "."
    for (size_t i = 0; i < sizeof(output_file)-1; i++)  // Looping through the part of the output file name before the "."
    {
        buff[i] = output_file[i];   // Storing encountered characters in variable
    }
    //Finish storing the part of the output file name before the "."

    root_node = xmlNewNode(NULL, BAD_CAST buff);   // Creating the root node of records XML file
    xmlDocSetRootElement(records_document, root_node);  // Assigning the previous root node as root node of the document

    customer readItem;

    fp = fopen(input_file, "r"); // Creating file pointer to read binary file

    if(fp == NULL)  // Checking if the file created correctly
    {
        printf("Error. Can't open binary file.");
        exit(1);
    }

    fseek(fp,0,SEEK_END);// seek to end of file
    int file_size = ftell(fp); // calculating the file size in bytes
    fseek(fp,0,SEEK_SET); // seek back to beginning of file


    for (size_t i = 1; i <= file_size/sizeof(customer); i++) // Looping through file
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
        xmlNewChild(customer_info_node, NULL, BAD_CAST "level_of_education", readItem.level_of_education); // Adding level_of_education node as child of customer_info node
        xmlNewChild(customer_info_node, NULL, BAD_CAST "email", readItem.email); // Adding email node as child of customer_info node

        xmlNewChild(bank_account_info_node, NULL, BAD_CAST "bank_account_number", readItem.bank_account_number); // Adding bank_account_number node as child of bank_account_info node
        xmlNewChild(bank_account_info_node, NULL, BAD_CAST "IBAN", readItem.IBAN); // Adding IBAN node as child of bank_account_info node
        xmlNewChild(bank_account_info_node, NULL, BAD_CAST "account_type", readItem.account_type); // Adding account_type node as child of bank_account_info node

        sprintf(buff, "%d", readItem.total_balance_available);
        total_balance_available_node = xmlNewChild(bank_account_info_node, NULL, BAD_CAST "total_balance_available", buff); // Adding total_balance_available node as child of bank_account_info node

        sprintf(buff, "%s", readItem.currency_unit);
        xmlNewProp(total_balance_available_node, BAD_CAST "currency_unit", BAD_CAST buff);  // Adding currency_unit attribute to bank_account_info node

        //Calculating little endian
        int little_endian;  // Creating little endian variable

	    char *big_endian_ptr = &readItem.total_balance_available; // Creating pointer for big endian(total_balance_available) value
	    char *little_endian_ptr = &little_endian;   // Creating pointer for little endian

        for (size_t j = 0; j < 4; j++)  // Since int is 4 byte, this loop will iterate all of integer's bytes
        {
            little_endian_ptr[j] = big_endian_ptr[3-j]; // Creating little endian value by reverse ordering the big endian bytes

        }
        //Finish calculating little endian

        sprintf(buff, "%d", little_endian);
        xmlNewProp(total_balance_available_node, BAD_CAST "bigEnd_Version", BAD_CAST buff);  // Adding bigEnd_Version attribute to bank_account_info node

        sprintf(buff, "%s", readItem.available_for_loan);
        xmlNewChild(bank_account_info_node, NULL, BAD_CAST "available_for_loan", buff); // Adding available_for_loan node as child of bank_account_info node
    }



    fclose(fp);

    //Dumping document to stdio and file
    xmlSaveFormatFileEnc(output_file, records_document, "UTF-8", 1);
    xmlSaveFormatFileEnc("-", records_document, "UTF-8", 1);


    //Free the document
    xmlFreeDoc(records_document);


    //Free the global variables that may have been allocated by the parser.
    xmlCleanupParser();
    
    //Memory Dump
    xmlMemoryDump(); 

} // end binary_to_XML


void XSD_validation(char* input_file, char* output_file){
    xmlDocPtr doc;
    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;
	
    char *XMLFileName = input_file; // write your xml file here
    char *XSDFileName = output_file; // write your xsd file here
    
    
    xmlLineNumbersDefault(1); //set line numbers, 0> no substitution, 1>substitution
    ctxt = xmlSchemaNewParserCtxt(XSDFileName); //create an xml schemas parse context
    schema = xmlSchemaParse(ctxt); //parse a schema definition resource and build an internal XML schema
    xmlSchemaFreeParserCtxt(ctxt); //free the resources associated to the schema parser context
    
    doc = xmlReadFile(XMLFileName, NULL, 0); //parse an XML file
    if (doc == NULL)
    {
        fprintf(stderr, "Could not parse %s\n", XMLFileName);
    }
    else
    {
        xmlSchemaValidCtxtPtr ctxt;  //structure xmlSchemaValidCtxt, not public by API
        int ret;
        
        ctxt = xmlSchemaNewValidCtxt(schema); //create an xml schemas validation context 
        ret = xmlSchemaValidateDoc(ctxt, doc); //validate a document tree in memory
        if (ret == 0) //validated
        {
            printf("%s validates\n", XMLFileName);
        }
        else if (ret > 0) //positive error code number
        {
            printf("%s fails to validate\n", XMLFileName);
        }
        else //internal or API error
        {
            printf("%s validation generated an internal error\n", XMLFileName);
        }
        xmlSchemaFreeValidCtxt(ctxt); //free the resources associated to the schema validation context
        xmlFreeDoc(doc);
    }
    // free the resource
    if(schema != NULL)
        xmlSchemaFree(schema); //deallocate a schema structure

    xmlSchemaCleanupTypes(); //cleanup the default xml schemas types library
    xmlCleanupParser(); //cleans memory allocated by the library itself 
    xmlMemoryDump(); //memory dump

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
