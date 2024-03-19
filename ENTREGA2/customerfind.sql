SELECT
customername, contactfirstname, contactlastname, customernumber
FROM
customers
WHERE
contactfirstname like '%a%' --por ejemplo
OR
contactlastname like '%a%'

ORDER BY
customernumber;