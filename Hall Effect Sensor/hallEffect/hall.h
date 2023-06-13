int getCount(int count)
{
  int val = hallRead();
  Serial.println(val);
  delay(1000);

  if(val > 40 || val < 0) {
    count++;
  }

  return count;
}

int pointsReturn(int count, int points)
{
  if((count % 10 == 0) && count != 0) {
    points += 10;
  }

  return points;
}
