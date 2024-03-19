SELECT
ordernumber, orderdate, shippeddate
FROM
orders
WHERE
orderdate BETWEEN '2003-01-01' AND '2003-12-30'
ORDER BY
ordernumber;
