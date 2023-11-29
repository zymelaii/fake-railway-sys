CREATE TABLE account
(
    user_id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL,
    password TEXT NOT NULL
);

CREATE TABLE schedule
(
    train_id INTEGER PRIMARY KEY AUTOINCREMENT,
    departure TEXT NOT NULL,
    destination TEXT NOT NULL,
    departure_time DATETIME NOT NULL,
    arrival_time DATETIME NOT NULL,
    ticket_price REAL NOT NULL
);

CREATE TABLE tickets
(
    ticket_id INTEGER PRIMARY KEY AUTOINCREMENT,
    train_id INTEGER,
    user_id INTEGER,
    purchase_time DATETIME NOT NULL,
    is_group BOOLEAN NOT NULL,
    ticket_price REAL NOT NULL,
    FOREIGN KEY (train_id) REFERENCES schedule(train_id),
    FOREIGN KEY (user_id) REFERENCES account(user_id)
);

CREATE TABLE integrate (
    record_id INTEGER PRIMARY KEY AUTOINCREMENT,
    ticket_id INTEGER,
    group_size INTEGER NOT NULL,
    FOREIGN KEY (ticket_id) REFERENCES tickets(ticket_id)
);
