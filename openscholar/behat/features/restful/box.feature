Feature:
  Testing boxes.

  @api @restful @bar
  Scenario: CRUD-ind a box.
    Given I "create" a box as "john" with the settings:
      | Site    | Widget  | Description  |
      | john    | Terms   | Terms        |
    When I "update" a box as "john" with the settings:
      | Site    | Widget  | Description  | Delta |
      | john    | Terms   | Terms - new  | PREV  |
    Then I "delete" a box as "john" with the settings:
      | Site    | Widget  | Description  | Delta |
      | john    | Terms   | Terms - new  | PREV  |
