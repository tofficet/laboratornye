#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_USERS 100
#define MAX_INPUT 100

typedef struct {
    char login[7];
    int pin;
    int request_limit;
    int request_count;
} User;

typedef struct {
    User users[MAX_USERS];
    int user_count;
    int current_user_index;
} ShellState;

int isvalidlogin(const char *login) {
    for (int i = 0; login[i] != '\0'; i++) {
        if (!isalnum(login[i]) || i >= 6) {
            return 0;
        }
    }
    return (strlen(login) > 0);
}

int is_login_unique(const ShellState *state, const char *login) {
    for (int i = 0; i < state->user_count; i++) {
        if (strcmp(state->users[i].login, login) == 0) return 0;
    }
    return 1;
}

void howmuch_command(const char* datetime, const char* flag) {
    struct tm specified_time = {0};
    time_t now_time = time(NULL);
    
    if (sscanf(datetime, "%d:%d:%d %d:%d:%d", 
               &specified_time.tm_mday, &specified_time.tm_mon, &specified_time.tm_year,
               &specified_time.tm_hour, &specified_time.tm_min, &specified_time.tm_sec) != 6) {
        printf("Invalid datetime format\n");
        return;
    }
    
    specified_time.tm_mon -= 1;
    specified_time.tm_year -= 1900;
    
    time_t spec_time = mktime(&specified_time);
    if (spec_time == -1) {
        printf("Invalid datetime\n");
        return;
    }
    
    double diff_seconds = difftime(now_time, spec_time);
    
    if (strcmp(flag, "-s") == 0) printf("Seconds: %.0f\n", diff_seconds);
    else if (strcmp(flag, "-m") == 0) printf("Minutes: %.0f\n", diff_seconds / 60);
    else if (strcmp(flag, "-h") == 0) printf("Hours: %.2f\n", diff_seconds / 3600);
    else if (strcmp(flag, "-y") == 0) printf("Years: %.2f\n", diff_seconds / (365.25 * 24 * 3600));
    else printf("Invalid flag\n");
}

void sanctions_command(ShellState *state, const char* username) {
    int confirm;
    printf("Enter 52 to confirm: ");
    scanf("%d", &confirm);
    while (getchar() != '\n');
    
    if (confirm != 52) {
        printf("Confirmation failed\n");
        return;
    }
    
    for (int i = 0; i < state->user_count; i++) {
        if (strcmp(state->users[i].login, username) == 0) {
            state->users[i].request_limit = 5;
            printf("Sanctions applied\n");
            return;
        }
    }
    printf("User not found\n");
}

int display(ShellState *state) {
    char input[MAX_INPUT];
    int choice;
    
    while (1) {
        printf("1.Time\n2.Date\n3.Howmuch\n4.Logout\n5.Sanctions\nChoice: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        if (sscanf(input, "%d", &choice) != 1) continue;
        
        if (state->users[state->current_user_index].request_limit != -1 &&
            state->users[state->current_user_index].request_count >= 
            state->users[state->current_user_index].request_limit) {
            printf("Request limit exceeded\n");
            return 0;
        }
        
        state->users[state->current_user_index].request_count++;
        
        switch (choice) {
            case 1: {
                time_t mytime = time(NULL);
                struct tm *now = localtime(&mytime);
                printf("Time: %02d:%02d:%02d\n", now->tm_hour, now->tm_min, now->tm_sec);
                return 1;
            }
            case 2: {
                time_t mytime = time(NULL);
                struct tm *now = localtime(&mytime);
                printf("Date: %02d.%02d.%d\n", now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
                return 1;
            }
            case 3: {
                char datetime[50], flag[10];
                printf("Enter datetime: ");
                fgets(datetime, sizeof(datetime), stdin);
                datetime[strcspn(datetime, "\n")] = 0;
                printf("Enter flag: ");
                fgets(flag, sizeof(flag), stdin);
                flag[strcspn(flag, "\n")] = 0;
                howmuch_command(datetime, flag);
                return 1;
            }
            case 4:
                return 0;
            case 5: {
                char username[7];
                printf("Enter username: ");
                fgets(username, sizeof(username), stdin);
                username[strcspn(username, "\n")] = 0;
                sanctions_command(state, username);
                return 1;
            }
            default:
                continue;
        }
    }
}

int register_user(ShellState *state) {
    char login[7], input[50];
    int pin;
    
    printf("Enter login: ");
    fgets(login, sizeof(login), stdin);
    login[strcspn(login, "\n")] = 0;
    
    if (!isvalidlogin(login)) {
        printf("Invalid login\n");
        return 0;
    }
    
    if (!is_login_unique(state, login)) {
        printf("Login exists\n");
        return 0;
    }
    
    printf("Enter PIN: ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%d", &pin) != 1 || pin < 0 || pin > 100000) {
        printf("Invalid PIN\n");
        return 0;
    }
    
    User new_user;
    strcpy(new_user.login, login);
    new_user.pin = pin;
    new_user.request_limit = -1;
    new_user.request_count = 0;
    
    state->users[state->user_count] = new_user;
    state->user_count++;
    
    printf("Registration successful\n");
    return 1;
}

int login_user(ShellState *state) {
    char login[7], input[50];
    int pin;
    
    printf("Enter login: ");
    fgets(login, sizeof(login), stdin);
    login[strcspn(login, "\n")] = 0;
    
    printf("Enter PIN: ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%d", &pin) != 1) {
        printf("Invalid PIN\n");
        return -1;
    }
    
    for (int i = 0; i < state->user_count; i++) {
        if (strcmp(state->users[i].login, login) == 0 && state->users[i].pin == pin) {
            state->users[i].request_count = 0;
            return i;
        }
    }
    printf("Invalid login or PIN\n");
    return -2;
}

void home(ShellState *state) {
    char input[50];
    int choice;
    
    while (1) {
        printf("1.Registration\n2.Log in\n3.Exit\nChoice: ");
        
        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        if (sscanf(input, "%d", &choice) != 1) continue;
        
        switch (choice) {
            case 1:
                register_user(state);
                break;
            case 2:
                state->current_user_index = login_user(state);
                if (state->current_user_index != -1) {
                    while (display(state));
                    state->current_user_index = -1;
                }
                break;
            case 3:
                return; 
        }
    }
}

int main() {
    ShellState state;
    state.user_count = 0;
    state.current_user_index = -1;
    
    for (int i = 0; i < MAX_USERS; i++) {
        state.users[i].login[0] = '\0';
        state.users[i].pin = 0;
        state.users[i].request_limit = -1;
        state.users[i].request_count = 0;
    }
    
    home(&state);
    return 0;
}