# gQSL Ham Radio QSL Logging Software and API

## Phase 1: Standalone Application (C & GTK)
### 1. Project Goals and Scope:
- **Goal:** Develop a user-friendly, standalone desktop application for ham radio operators to log QSL contacts in ADIF format.
**Scope:**
- Create a GTK-based GUI adhering to GNOME HIG.
- Implement data entry fields for essential QSL information (Callsign, Date, Time, Frequency, Mode, RST, etc.).
- Support ADIF file import and export.
- Provide basic log management features (viewing, editing, deleting entries).
- Ensure platform compatibility (Linux primarily, with potential for cross-platform considerations).

## 2. Functional Requirements:
**Data Entry:**  
    - Intuitive GUI for entering QSL contact details.  
    - Input validation to ensure data integrity (e.g., date/time formats, frequency ranges).  
    - Automatic timestamping.  

**Log Management:**  
    - Display logs in a tabular format (e.g., GtkTreeView).  
    - Sorting and filtering capabilities.  
    - Editing and deleting individual log entries.  
    - Search functionality.  

**ADIF Support:**  
    - Import logs from existing ADIF files.  
    - Export logs to ADIF files.  
    - Correctly handle ADIF field mappings.  

**User Interface:**  
    - Adhere to GNOME HIG for consistent look and feel.  
    - Use clear and concise labels and tooltips.  
    - Provide user feedback for operations (e.g., confirmation messages).  
    - Use a preferences window to set user callsign information.  


## 3. Technical Design:
- **Programming Language: C.**
- **GUI Toolkit:** GTK (version 4 is recommended for modern GNOME look and feel).
- **Data Storage (Local):**
- In-memory data structures for log entries.
- File-based storage (ADIF files) for persistence.
- **ADIF Parsing/Generation:**
- Use a dedicated ADIF library (if available) or implement custom parsing/generation functions.
- **Build System:**
- Use Meson for build configuration.
- Use Git for version control.


## 4. Development Workflow:
- **Iterative Development:** Implement features in small, manageable increments.
- **Version Control:** Use Git for code management and collaboration.
- **Testing:** Implement unit tests and integration tests.
- **Code Reviews:** Conduct regular code reviews to ensure quality.


## 5. Project Timeline (Estimate):
- Phase 1 (Standalone Application): 4-8 weeks (adjust based on complexity and available time).
  


# Phase 2: Server-Based API and Database

## 1. Project Goals and Scope:
- **Goal:** Extend the standalone application with server-based storage and API for remote QSL logging and confirmation.
- **Scope:**
- Set up a database (e.g., PostgreSQL, MySQL) on a VPS server.
- Develop a RESTful API for accessing and manipulating log data.
- Modify the standalone application to interact with the API.
- Implement QSL confirmation functionality.

## 2. Functional Requirements:
- **Database:**
- Store log entries in a relational database.
- Design a database schema that efficiently stores QSL data.
- Database should store user callsign information.
- **API:**
- Provide endpoints for creating, reading, updating, and deleting log entries.
- Implement authentication and authorization for API access.
- Implement an endpoint for QSL confirmation.
- Return data in JSON format.
- **Standalone Application (Client):**
- Modify the application to use the API for data storage and retrieval.
- Implement QSL confirmation functionality (e.g., sending confirmation requests, receiving confirmations).
- Implement user account managment.
- **QSL Confirmation:**
- Allow users to request QSL confirmation from other users.
- Allow users to confirm received QSL requests.
- Store confirmation status in the database.

## 3. Technical Design:
- **Database:**
- PostgreSQL or MySQL (select based on familiarity and requirements).
- **API Framework:**
- C with a web framework (e.g., libmicrohttpd, or a C++ framework and C bindings).
- **API Protocol:**
- RESTful API using JSON for data exchange.
- **Server:**
- Linux VPS.
- **Authentication/Authorization:**
- JWT or OAuth 2.0.

## 4. Development Workflow:
- **Backend Development:** Develop the database and API first.
- **Frontend Integration:** Modify the standalone application to integrate with the API.
- **Testing:** Implement API tests and integration tests.
- **Deployment:** Deploy the API and database to the VPS server.

## 5. Project Timeline (Estimate):
- Phase 2 (Server-Based API and Database): 6-12 weeks (adjust based on complexity and available time).
