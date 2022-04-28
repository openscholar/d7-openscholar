Feature:
  Testing the stats feature JSON ouput.

  @api @wip
  Scenario: Verify for the json output for a specific node.
    Given I visit "stats"
      And I should get:
      """
      {"success":true,"filesize":{"value":"{{*}}","text":"Total uploaded"},"filesize_bytes":{"value":"{{*}}","text":"Total uploaded bytes"},"users":{"value":"{{*}}","text":"Users"},"websites":{"value":"{{*}}","text":"Websites"},"posts":{"value":"{{*}}","text":"Posts"},"publications":{"value":"{{*}}","text":"Publications"},"files":{"value":"{{*}}","text":"Uploaded files"},"href":"{{*}}","os_version":"{{*}}"}
      """

  @api @misc_second
  Scenario: Verify for the json output for a specific node.
    Given I visit "geckoboard"
    And I should get:
    """
    {"item":[{"value":"{{*}}","text":""}]}
    """
    When I visit "stats?style=geckoboard&type=websites"
    Then I should get:
    """
    {"item":[{"value":"{{*}}","text":""}]}
    """
