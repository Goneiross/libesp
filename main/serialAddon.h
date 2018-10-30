/*
    Enter your command and help here
*/

void specifiHelp(){

}

void specificCommand(char* data){

}
/*
    Built in commands
*/ 

void settinsHelp(){

}

void settingsCommand(char* data){
    switch(data[1]){
        default :
            settingsHelp();
    }
}

void utilityHelp(){
    printf("ur - reset");
}

void utilityCommand(char* data){
    switch(data[1]){
        case 'r':
            esp_restart();
        default :
            utilityHelp();
    }
}

