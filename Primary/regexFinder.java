import java.io.BufferedWriter;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.Writer;
import java.util.Scanner;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class regexFinder
{
	public static void main(String[] args)
			throws IOException, java.io.FileNotFoundException
	{
		int i, j, e, y, t, d, v, counter = 0, temp = 0;
		int request = 5;
		int nodes = 7;
		int[][] a = new int[nodes][nodes + 1];
		int l = 0;

		String fileName = "D:/Thesis/Primary/solution.txt";
		String fileName1 = "D:/Thesis/Primary/LightpathInfo.txt";
		String line = "";
		String line1 = "";
		Scanner scanner = new Scanner(new FileReader(fileName));
		Scanner scanner1 = new Scanner(new FileReader(fileName1));

		while (scanner1.hasNextInt())
		{
			for (i = 0; i < request; i++)
			{
				for (j = 0; j < nodes; j++)
					a[i][j] = scanner1.nextInt();
			}
		}

		// print the input matrix

		System.out.println("The input sorted matrix is : ");
		for (i = 0; i < request; i++)
		{
			for (j = 0; j < nodes; j++)
			{
				System.out.print(a[i][j] + " ");
			}
			System.out.print("\n");
		}

		try
		{

			while (scanner.hasNextLine())
			{
				line = scanner.nextLine();

				// for ( i = 0; i < line.length(); i++) {
				Pattern p = Pattern.compile(
						"(X)\\w([0-9]\\d*)\\w([0-9]\\d*)\\w([0-9]\\d*)");

				Pattern p1 = Pattern.compile("(V)\\w([0-9]\\d*)\\w([0-9]\\d*)");

				Matcher m = p.matcher(line);

				Matcher m1 = p1.matcher(line);

				// now try to find at least one match
				if (m.find())
				{

					Writer writer = null;
					writer = new BufferedWriter(new OutputStreamWriter(
							new FileOutputStream("D:/Thesis/Primary/temp.txt",
									false),
							"utf-8"));

					System.out.print("" + m.group(1));
					System.out.print("" + m.group(2));
					System.out.print("" + m.group(3));
					System.out.println("" + m.group(4));

					String h = m.group(2);
					String f = m.group(3);
					String w = m.group(4);

					t = Integer.parseInt(h);

					d = Integer.parseInt(f);
					v = Integer.parseInt(w);

					if (temp != t)
						counter = 0;
					a[t][counter] = d;
					a[t][counter + 1] = v;
					writer.write(a[t][counter] + " ");
					writer.write(a[t][counter + 1] + " ");
					System.out.println(a[t][counter]);
					System.out.println(a[t][counter + 1]);

					counter++;

					writer.close();
					temp = t;
				}

				if (m1.find())
				{

					Writer writer = null;
					writer = new BufferedWriter(new OutputStreamWriter(
							new FileOutputStream("D:/Thesis/Primary/temp.txt",
									false),
							"utf-8"));

					// System.out.print("" + m1.group(1));
					// System.out.print("" + m1.group(2));
					// System.out.print("" + m1.group(3));

					String h = m1.group(2);
					String f = m1.group(3);

					t = Integer.parseInt(h);
					d = Integer.parseInt(f);

					a[t][7] = d;
					writer.write(a[t][counter]);
					writer.close();
				}

			}

			Writer writer = null;
			writer = new BufferedWriter(new OutputStreamWriter(
					new FileOutputStream("D:/Thesis/Primary/temp.txt", false),
					"utf-8"));

			for (i = 0; i < request; i++)
			{

				for (j = 0; j < nodes + 1; j++)
				{

					writer.write(a[i][j] + " ");

					// System.out.print(a[i][j] + " ");
				}
				writer.write("\n");
			}
			writer.close();

		} finally
		{
			scanner.close();
		}

	}
}
