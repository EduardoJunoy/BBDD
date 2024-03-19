(SELECT
orderdate, status
FROM
orders
WHERE
ordernumber = 10100);


(SELECT
sum(quantityordered*priceeach)
FROM
orderdetails
where
ordernumber= 10100);



(SELECT
productcode, quantityordered, priceeach
from
orderdetails
WHERE
ordernumber= 10100;
);

SELECT orderdate, status FROM orders WHERE ordernumber = 10100;
SELECT sum(quantityordered*priceeach) FROM orderdetails where ordernumber= 10100;
SELECT productcode, quantityordered, priceeach from orderdetails WHERE ordernumber= 10100;