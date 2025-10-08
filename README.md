Mini Bank System
================

Overview
--------
Mini Bank System is an educational project that simulates the operation of a banking system with limited functionality.
It demonstrates object-oriented programming principles and client-server interaction using local Pipes as a communication mechanism.

Objective
---------
The main goal of the project is to create a simplified banking environment that allows users and administrators to interact with a local server and perform basic banking operations.

Project Structure
-----------------
The system consists of three main applications:
1. User Client
2. Admin Client
3. Server Client

The User and Admin clients communicate with the Server Client through local Pipes (temporary files).
A full network server implementation is planned for the extended stage of the project.

Main Features (Stage 1)
-----------------------
1. Server-side database implemented using binary files.
2. User management system (login, logout, profile update).
3. Bank account management (creation, deposits, withdrawals, transfers).
4. Support for different account types: default, credit, deposit.
5. Logging system for all database and banking operations.
6. Admin client for managing users, accounts, and transactions.
7. Simple text-based interface for User and Admin clients.

Future Development (Stage 2)
----------------------------
1. Financial reporting in CSV format.
2. Automated data backups and security improvements.
3. Full server implementation using sockets.

Author and Year
---------------
Developed as part of an Object-Oriented Programming course project.
Year: 2025
