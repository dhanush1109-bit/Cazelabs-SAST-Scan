/**
 * ======================================================
 *  VULNERABLE SAMPLE APPLICATION — FOR SAST TESTING ONLY
 *  DO NOT USE IN PRODUCTION
 *  Contains: SQL Injection, XSS, Hardcoded Secrets,
 *            Buffer Overflow, Command Injection
 * ======================================================
 */

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <stdio.h>

// -------------------------------------------------------
// VULNERABILITY 1: HARDCODED SECRETS & CREDENTIALS
// SonarQube will flag these as "Security Hotspots"
// -------------------------------------------------------
const char* DB_HOST       = "192.168.1.100";
const char* DB_USER       = "admin";
const char* DB_PASSWORD   = "SuperSecret123!";   // Hardcoded password
const char* API_KEY       = "sk-abc123hardcodedAPIkey9999";  // Hardcoded API key
const char* SECRET_TOKEN  = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.hardcoded"; // JWT token hardcoded

// -------------------------------------------------------
// VULNERABILITY 2: SQL INJECTION
// User input is directly concatenated into SQL query
// without sanitization or parameterized queries
// -------------------------------------------------------
std::string buildLoginQuery(const std::string& username, const std::string& password) {
    // BAD: Direct string concatenation — classic SQL Injection
    std::string query = "SELECT * FROM users WHERE username='" 
                        + username 
                        + "' AND password='" 
                        + password + "';";
    
    std::cout << "[DB] Executing query: " << query << std::endl;
    return query;
}

// Simulated database execution (no actual DB, just for SAST demonstration)
bool loginUser(const std::string& username, const std::string& password) {
    std::string query = buildLoginQuery(username, password);
    // Imagine this runs against a real DB:
    // mysql_query(conn, query.c_str());  <-- SQL Injection right here
    return true;
}

// -------------------------------------------------------
// VULNERABILITY 3: CROSS-SITE SCRIPTING (XSS)
// User input is directly embedded into HTML output
// without encoding or escaping
// -------------------------------------------------------
void renderHTMLPage(const std::string& userInput) {
    // BAD: Directly embedding raw user input into HTML
    std::string html = "<html><body>"
                       "<h1>Welcome, " + userInput + "!</h1>"  // XSS here
                       "<p>Your search: <b>" + userInput + "</b></p>"  // XSS again
                       "</body></html>";
    
    std::cout << "[HTML Output]:" << std::endl;
    std::cout << html << std::endl;
    // In a real web server this would be sent to the browser
    // allowing attackers to inject: <script>alert('XSS')</script>
}

// -------------------------------------------------------
// VULNERABILITY 4: BUFFER OVERFLOW
// Using unsafe functions like strcpy, gets, sprintf
// without bounds checking
// -------------------------------------------------------
void processUsername(const char* input) {
    char buffer[50];             // Fixed small buffer
    strcpy(buffer, input);       // BAD: No bounds check — Buffer Overflow
    printf("Username: %s\n", buffer);
}

void readUserInput() {
    char buf[100];
    gets(buf);                   // BAD: gets() is inherently unsafe, removed in C11
    printf("You entered: %s\n", buf);
}

// -------------------------------------------------------
// VULNERABILITY 5: COMMAND INJECTION
// User input is passed directly into system() call
// -------------------------------------------------------
void runDiagnostic(const std::string& hostname) {
    // BAD: Attacker can inject shell commands
    // e.g., hostname = "google.com; rm -rf /"
    std::string command = "ping " + hostname;
    system(command.c_str());     // Command Injection vulnerability
}

// -------------------------------------------------------
// VULNERABILITY 6: FORMAT STRING VULNERABILITY
// -------------------------------------------------------
void logMessage(const char* userMessage) {
    printf(userMessage);         // BAD: Should be printf("%s", userMessage)
    // Attacker can pass "%x %x %x" to leak stack memory
}

// -------------------------------------------------------
// VULNERABILITY 7: USE OF DEPRECATED / UNSAFE FUNCTIONS
// -------------------------------------------------------
void unsafeCopy(const char* src) {
    char dest[20];
    sprintf(dest, src);          // BAD: No size limit, format string risk
    strcat(dest, " - processed");// BAD: No bounds check
}

// -------------------------------------------------------
// VULNERABILITY 8: NULL POINTER DEREFERENCE
// -------------------------------------------------------
void processData(int* data) {
    // BAD: No null check before dereferencing
    int value = *data;           // Crash if data is NULL
    std::cout << "Value: " << value << std::endl;
}

// -------------------------------------------------------
// VULNERABILITY 9: MEMORY LEAK
// -------------------------------------------------------
void fetchUserProfile(int userId) {
    char* buffer = new char[1024];
    sprintf(buffer, "User profile for ID: %d", userId);
    std::cout << buffer << std::endl;
    // BAD: Missing delete[] buffer — Memory Leak
}

// -------------------------------------------------------
// MAIN — Simulates user interaction
// -------------------------------------------------------
int main() {
    std::cout << "===============================" << std::endl;
    std::cout << "  VULNERABLE APP — SAST DEMO   " << std::endl;
    std::cout << "===============================" << std::endl;

    // Simulate SQL Injection scenario
    std::string username, password;
    std::cout << "\n[LOGIN] Enter username: ";
    std::cin >> username;
    std::cout << "[LOGIN] Enter password: ";
    std::cin >> password;
    loginUser(username, password);

    // Simulate XSS scenario
    std::string searchQuery;
    std::cout << "\n[SEARCH] Enter search term: ";
    std::cin >> searchQuery;
    renderHTMLPage(searchQuery);

    // Simulate Command Injection scenario
    std::string host;
    std::cout << "\n[DIAGNOSTIC] Enter hostname to ping: ";
    std::cin >> host;
    runDiagnostic(host);

    // Simulate Buffer Overflow scenario
    char longInput[] = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    processUsername(longInput);

    // Simulate null pointer dereference
    int* nullPtr = nullptr;
    // processData(nullPtr);  // Commented out so it doesn't actually crash

    // Simulate memory leak
    fetchUserProfile(42);

    std::cout << "\n[INFO] API Key in use: " << API_KEY << std::endl;
    std::cout << "[INFO] DB Password: " << DB_PASSWORD << std::endl;

    return 0;
}
