Feature:
  Testing the dataverse widget.

  @api @widgets @javascript
  Scenario: Verify the Dataverse List widget.
     Given I am logging in as "john"
     And I create a "Dataverse List" widget for the vsite "john" with the following <settings>:
          | edit-description  | Dataverse List | textfield   |
          | edit-title        | Dataverse List | textfield   |
          | edit-user-alias   | harvard        | textfield   |
    When the dataverse widget "Dataverse List" is placed in the "About" layout
     And I visit "john/about"
     Then I should see "Dataverse List"

  @api @widgets @javascript
  Scenario: Verify the Dataverse Search Box widget.
     Given I am logging in as "john"
     And I create a "Dataverse Search Box" widget for the vsite "john" with the following <settings>:
          | edit-description  | Dataverse Search Box| textfield   |
          | edit-title        | Dataverse Search Box| textfield   |
          | edit-user-alias   | harvard             | textfield   |
    When the dataverse widget "Dataverse Search Box" is placed in the "About" layout
      And I visit "john/about"
     Then I should see "Dataverse Search Box"

  @api @widgets @javascript
  Scenario: Verify the Dataverse Dataset Citation Box widget.
     Given I am logging in as "john"
      And I create a "Dataverse Dataset Citation" widget for the vsite "john" with the following <settings>:
          | edit-description     | Dataverse Dataset Citation | textfield   |
          | edit-title           | Dataverse Dataset Citation | textfield   |
          | edit-persistent-id   | 10.7910/DVN/AURKTO         | textfield   |
    When the dataverse widget "Dataverse Dataset Citation" is placed in the "About" layout
      And I visit "john/about"
     Then I should see "Dataverse Dataset Citation"

  @api @widgets @javascript
  Scenario: Verify the Dataverse Dataset Box widget.
     Given I am logging in as "john"
      And I create a "Dataverse Dataset" widget for the vsite "john" with the following <settings>:
          | edit-description     | Dataverse Dataset   | textfield   |
          | edit-title           | Dataverse Dataset   | textfield   |
          | edit-persistent-id   | 10.7910/DVN/AURKTO  | textfield   |
    When the dataverse widget "Dataverse Dataset" is placed in the "About" layout
      And I visit "john/about"
     Then I should see "Dataverse Dataset"
