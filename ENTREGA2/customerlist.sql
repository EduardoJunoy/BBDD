SELECT
productname, sum(quantityordered)
FROM
customers
natural join
orders
natural join
orderdetails
natural join
products
WHERE
customernumber = 103

GROUP BY
productname, productcode

ORDER BY
productcode;

SELECT productname, sum(quantityordered) FROM customers natural join orders natural join orderdetails natural join products WHERE customernumber = 103 GROUP BY productname, productcode ORDER BY productcode;