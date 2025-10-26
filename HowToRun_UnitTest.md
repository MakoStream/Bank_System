# User Client Unit Test

## 1. About the Unit Test
This project includes a **unit test for the User Client**, which interacts with a server by sending various commands and verifying the responses. The test ensures that the server and client communication works as expected, particularly focusing on user-related commands like:

- User login and registration.
- Session handling (getting session ID, logging in).
- User account management (creating users, retrieving user info).
- Database creation for users and accounts.

The test is implemented using the **Boost Unit Test Framework** and is designed to simulate interactions with the server and analyze the received data to ensure correctness.

Each unit test is aimed at testing individual commands, such as `login`, `reg_user`, `me`, `user_list`, and others. The main purpose of this unit test is to validate the server's response for various client commands, checking both expected success and error messages.

The following core functions are tested:
- **Login tests** (valid and invalid login scenarios)
- **Registration tests** (new users and existing users)
- **User information retrieval**
- **Account creation and management**

---

## 2. How to Run the Unit Test

### Step 1: Set Up the Environment
1. **Clone the repository** that contains both the server and user client code.
2. **Create a `debug` folder** in the root of the server repository. This will be used to store the debug build of the server and user client.

### Step 2: Build the Projects
1. **Open the server project** and build it in **debug mode** using your IDE or command line.
   - If you're using Visual Studio, open the solution and build it in **Debug** configuration.
2. **Open the user client project** and also build it in **debug mode**.
   
   Both projects must be built in **debug mode** to ensure that debugging information is available when running the tests.

### Step 3: Start the Server and User Client
1. **Start the server** in a terminal or your IDE. Make sure the server is running and listening for client connections.
2. **Start the user client** in another terminal or IDE instance. The client will send requests to the server and run the tests.

### Step 4: Run the Unit Test
1. **Activate the debug environment** in the user client by entering the command `debug`. This will set up the environment for safe operations and debugging.
2. Once the debug environment is initialized, run the unit tests by entering `unit_test`

### Step 5: remove *.dat files
1. Remove accounts.dat from `debug` folder.
2. Remove users.dat from `debug` folder.
