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
    ticket_price REAL NOT NULL,
    capacity INTEGER NOT NULL
);

CREATE TABLE tickets
(
    ticket_id INTEGER PRIMARY KEY AUTOINCREMENT,
    train_id INTEGER,
    user_id INTEGER,
    purchase_time DATETIME NOT NULL,
    is_group BOOLEAN NOT NULL,
    FOREIGN KEY (train_id) REFERENCES schedule(train_id),
    FOREIGN KEY (user_id) REFERENCES account(user_id)
);

CREATE TABLE integrate (
    record_id INTEGER PRIMARY KEY AUTOINCREMENT,
    ticket_id INTEGER,
    group_size INTEGER NOT NULL,
    FOREIGN KEY (ticket_id) REFERENCES tickets(ticket_id) ON DELETE CASCADE
);

CREATE VIEW view_remain_tickets AS
SELECT
    schedule.train_id,
    capacity - (COALESCE(alone_total, 0) + COALESCE(group_total, 0)) AS total
FROM schedule
LEFT JOIN (
    SELECT train_id, COUNT(*) AS alone_total
    FROM tickets
    WHERE is_group = 'false'
    GROUP BY train_id
) alone_acc ON schedule.train_id = alone_acc.train_id
LEFT JOIN (
    SELECT train_id, SUM(integrate.group_size) AS group_total
    FROM tickets
    LEFT JOIN integrate ON tickets.ticket_id = integrate.ticket_id
    WHERE is_group = 'true'
    GROUP BY train_id
) group_acc ON schedule.train_id = group_acc.train_id;
