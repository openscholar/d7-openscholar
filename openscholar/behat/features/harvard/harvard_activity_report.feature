Feature:
  In order to see a site biblio publications
  I need to be able to issue a query via URL
  And get XML results

  @api @harvard
  Scenario: Test a query with unknown VSite, where the answer should be "unknown".
    Given I visit "harvard_activity_reports"
    Then I should get:
    """
    <?xml version="1.0" encoding="UTF-8"?>
    <response xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespacesSchemaLocation="far_response.xsd">
      <person huid="" sourceUrl="" action_status="unknown"/>
    </response>
    """

  @api @harvard
  Scenario: Test a query with invalid VSite, where the answer should be "error".
    Given I visit "harvard_activity_reports?id=foo"
    Then I should get:
    """
    <?xml version="1.0" encoding="UTF-8"?>
    <response xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespacesSchemaLocation="far_response.xsd">
      <person huid="foo" sourceUrl="" action_status="error"/>
    </response>
    """

  @api @wip
  Scenario: Test a query withing a VSite for a year with publication, where the answer should be "ok".
    Given I visit "john/harvard_activity_reports?year=1943"
    Then I should get:
    """
    <?xml version="1.0" encoding="UTF-8"?>
    <response xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespacesSchemaLocation="far_response.xsd">
      <person huid="" sourceUrl="{{*}}" action_status="ok">
        <publication id="{{*}}" pubType="Book" pubSource="OpenScholar">
          <citation>. The Little Prince. </citation>
          <linkToArticle></linkToArticle>
          <yearOfPublication>1943</yearOfPublication>
    """

  @api @harvard
  Scenario: Test a query withing a VSite for a year with no publication, where the answer should be "ok".
    Given I visit "john/harvard_activity_reports"
    Then I should get:
    """
    <?xml version="1.0" encoding="UTF-8"?>
    <response xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xsi:noNamespacesSchemaLocation="far_response.xsd">
      <person huid="" sourceUrl="{{*}}" action_status="ok"/>
    </response>
    """
