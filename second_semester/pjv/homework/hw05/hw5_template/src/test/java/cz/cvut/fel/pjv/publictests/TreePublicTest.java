package cz.cvut.fel.pjv.publictests;

import cz.cvut.fel.pjv.Node;
import cz.cvut.fel.pjv.Tree;
import cz.cvut.fel.pjv.impl.TreeImpl;
import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.DisplayName;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.params.ParameterizedTest;
import org.junit.jupiter.params.provider.Arguments;
import org.junit.jupiter.params.provider.MethodSource;

import java.util.stream.Stream;

@DisplayName("Tree building - public tests")
class TreePublicTest {

	@Test
	void testTreeEmpty() {
		Tree tree = new TreeImpl();
		tree.setTree(new int[0]);

		Node root = tree.getRoot();
		Assertions.assertNull(root, "Root of empty tree is expected to be null, but it is %s".formatted(root));
		Assertions.assertEquals("", tree.toString(), "String representation of empty tree is expected to be empty, but it is %n-----%n%s".formatted(tree.toString()));
	}

	@ParameterizedTest
	@MethodSource("getTreeInputs")
	void testPublicTree(int[] values, int expectedRoot, String expectedString) {
		Tree tree = new TreeImpl();
		tree.setTree(values);

		Node root = tree.getRoot();
		Assertions.assertNotNull(root, "Root of tree is expected to be not null, but it is %s".formatted(root));
		Assertions.assertEquals(expectedRoot, root.getValue(), "Root of tree is expected to be %d, but it is %d".formatted(expectedRoot, root.getValue()));
		Assertions.assertEquals(expectedString, tree.toString(), "String representation of tree is expected to be %n-----%n%s%n-----%n, but it is %n-----%n%s".formatted(expectedString, tree.toString()));
	}

	static Stream<Arguments> getTreeInputs() {
		return Stream.of(
				Arguments.of(new int[]{1}, 1, "- 1\n"),
				Arguments.of(new int[]{1, 2}, 2, """
						- 2
						 - 1
						 """),
				Arguments.of(new int[]{1, 2, 3}, 2, """
						- 2
						 - 1
						 - 3
						 """),
				Arguments.of(new int[]{1, 2, 3, 4}, 3, """
						- 3
						 - 2
						  - 1
						 - 4
						 """),
				Arguments.of(new int[]{1, 2, 3, 4, 5}, 3, """
						- 3
						 - 2
						  - 1
						 - 5
						  - 4
						  """),
				Arguments.of(new int[]{1, 2, 3, 4, 5, 6}, 4, """
						- 4
						 - 2
						  - 1
						  - 3
						 - 6
						  - 5
						  """),
				Arguments.of(new int[]{1, 2, 3, 4, 5, 6, 7}, 4, """
						- 4
						 - 2
						  - 1
						  - 3
						 - 6
						  - 5
						  - 7
						  """),
				Arguments.of(new int[]{1, 2, 3, 4, 5, 6, 7, 8}, 5, """
						- 5
						 - 3
						  - 2
						   - 1
						  - 4
						 - 7
						  - 6
						  - 8
						  """),
				Arguments.of(new int[]{1, 2, 3, 4, 5, 6, 7, 8, 9}, 5, """
						- 5
						 - 3
						  - 2
						   - 1
						  - 4
						 - 8
						  - 7
						   - 6
						  - 9
						  """),
				Arguments.of(new int[]{1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 6, """
						- 6
						 - 3
						  - 2
						   - 1
						  - 5
						   - 4
						 - 9
						  - 8
						   - 7
						  - 10
						 """)
		);
	}

}
