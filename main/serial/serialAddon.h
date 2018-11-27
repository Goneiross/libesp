/*
    Enter your command and help here
*/

void specificHelp(){

}

void specificCommand(char* data){

}
/*
    Built in commands
*/ 

void settingsHelp(){

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

