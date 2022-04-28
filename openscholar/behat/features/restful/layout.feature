Feature:
  Test layout.

  @api @restful
  Scenario: CRUD-ing a layout.
    Given I "create" a layout as "john" with the settings:
      | Site | Context  | Box |
      | john | os_front | Pub |
    And I visit "john"
    And I should see the text "2014" under "region-sidebar-second"
    When I "update" a layout as "john" with the settings:
      | Site | Context  | Box | Delta |
      | john | os_front | Pub | PREV  |
    And I visit "john"
    And I should see the text "2014" under "region-sidebar-first"
    When I "delete" a layout as "john" with the settings:
      | Site | Context                  | Delta |
      | john | os_front:reaction:block  | PREV  |
    And I visit "john"
    And I should not see "2014"
