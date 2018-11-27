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