SELECT
sum(payments.amount)-sum(quantityordered*priceeach) as balance
FROM
customers
natural join
orders
natural join
orderdetails
natural join
payments

where customernumber = 103;

