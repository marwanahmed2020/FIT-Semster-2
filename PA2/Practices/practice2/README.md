# eRouška Contact Tracking — Practice 2

## Problem statement

Since the COVID pandemic is not under control yet, the government introduces new restrictions to slow the spread of the virus. It resurrected the idea of self-tracking by means of mobile phones (eRouška). You have been asked to prepare the software for this project. The project leader and software architect are unavailable, so you must both design the interface and implement the software.

You are expected to develop three classes. Some parts of the interface are fixed (see the provided test harness), but most design details are up to you. Your implementation must be compatible with the provided tests.

## Required classes (overview)

1. `CTimeStamp`
    - Represents a simple moment in time.
    - Required constructor signature: initialize year, month, day, hour, minute, second.
    - No validation of the values is required (inputs are guaranteed valid).

2. `CContact`
    - Encapsulates a single contact between two people.
    - Each person is identified by an integer phone number.
    - Each contact stores a `CTimeStamp` and the two phone numbers involved.

3. `CEFaceMask`
    - Stores contacts and provides search functionality.
    - `addContact(...)` adds a contact; calls should be chainable.
    - `listContacts(...)` returns a `std::vector<int>` with phone numbers the infected person met.
      - Overload A: `listContacts(phone)` — search by phone only.
      - Overload B: `listContacts(phone, fromTS, toTS)` — search limited to a time interval.
    - `listContacts` must be `const` (must not modify the object).

## Result requirements (detailed)

- Returned lists must contain no duplicates — include each phone number at most once (ignore later duplicates).
- Preserve the order in which contacts were added: earlier contacts appear before later ones.
- Do not include self-contacts (where both phone numbers are equal).

> Implementation note: phone numbers are represented as `int` here for simplicity (this is not recommended in real systems).

## Constraints and hints

- Example test data and harness are provided with the assignment; do not modify them.
- Optimizations are not required — a straightforward, naive implementation is fine.
- Keep class internals private; expose only the required methods.
- `std::vector`, `std::array`, `std::string`, `std::stringstream`, and standard algorithms are allowed.
- Avoid `std::ws` (some toolchains used in grading may behave unexpectedly with it).

## Testing and files

Create your implementation file inside this folder, for example:

- `practice2.cpp` or `erouska.cpp`

Implement the three classes and verify them using the provided test harness. The harness uses `assert` to check return values, so make sure `listContacts` returns exactly the expected `std::vector<int>` content and order.

If you want, I can also provide a starter skeleton that matches the test harness header and compiles immediately.
